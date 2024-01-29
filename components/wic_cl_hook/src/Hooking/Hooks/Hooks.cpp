#include <Windows.h>
#include <detours/detours.h>
#include <vector>
#include <memory>
#include "Hooks.h"
#include "Memory.h"

namespace Hooks
{
	struct CallbackEntry
	{
		const char *Name = nullptr;
		bool HasBooleanReturn = false;

		union
		{
			void (*VoidCallback)() = nullptr;
			bool (*BoolCallback)();
		};
	};

	struct HookTransactionEntry
	{
		void *DetourStubFunction = nullptr;
		void *TargetFunction = nullptr;
		bool RequiresCallFixup = false;
	};

	struct IATEnumContext
	{
		const char *ModuleName = nullptr;
		const char *ImportName = nullptr;
		const void *CallbackFunction = nullptr;
		bool ModuleFound = false;
	};

	std::vector<CallbackEntry>& GetInitializationEntries()
	{
		// Has to be a function-local static to avoid initialization order issues
		static std::vector<CallbackEntry> initializationEntries;
		return initializationEntries;
	}

	std::vector<std::unique_ptr<HookTransactionEntry>>& GetTransactionEntries()
	{
		static std::vector<std::unique_ptr<HookTransactionEntry>> transactionEntries;
		return transactionEntries;
	}

	bool Initialize()
	{
		OutputDebugStringA(__FUNCTION__ "()\n");
		DetourSetIgnoreTooSmall(true);

		// Begin the transaction
		if (DetourTransactionBegin() != NO_ERROR)
			return false;

		DetourUpdateThread(GetCurrentThread());

		for (const auto& callback : GetInitializationEntries())
		{
			OutputDebugStringA("Setting up hooks for ");
			OutputDebugStringA(callback.Name);
			OutputDebugStringA("...\n");

			if (callback.HasBooleanReturn)
			{
				// Bail on the whole process if a callback returns false
				if (!callback.BoolCallback())
				{
					DetourTransactionAbort();
					return false;
				}
			}
			else
			{
				// Continue unconditionally
				callback.VoidCallback();
			}
		}

		if (DetourTransactionCommit() != NO_ERROR)
			return false;

		// Apply fixups for calls
		for (const auto& transaction : GetTransactionEntries())
		{
			if (transaction->RequiresCallFixup)
				Memory::Patch(reinterpret_cast<std::uintptr_t>(transaction->TargetFunction), { 0xE8 });
		}

		GetInitializationEntries().clear();
		GetTransactionEntries().clear();

		OutputDebugStringA("Done!\n");
		return true;
	}

	void AddInitializationCallback(const char *Name, void(*Callback)())
	{
		GetInitializationEntries().emplace_back(CallbackEntry
		{
			.Name = Name,
			.HasBooleanReturn = false,
			.VoidCallback = Callback,
		});
	}

	void AddInitializationCallback(const char *Name, bool(*Callback)())
	{
		GetInitializationEntries().emplace_back(CallbackEntry
		{
			.Name = Name,
			.HasBooleanReturn = true,
			.BoolCallback = Callback,
		});
	}

	bool WriteJump(std::uintptr_t TargetAddress, const void *CallbackFunction, void **OriginalFunction)
	{
		if (!TargetAddress)
			return false;

		auto ptr = std::make_unique<HookTransactionEntry>();
		ptr->TargetFunction = reinterpret_cast<void *>(TargetAddress);

		// We need a temporary pointer until the transaction is committed
		if (!OriginalFunction)
			OriginalFunction = &ptr->DetourStubFunction;

		// Detours needs the real target function stored in said pointer
		*OriginalFunction = ptr->TargetFunction;

		if (DetourAttach(OriginalFunction, const_cast<void *>(CallbackFunction)) != NO_ERROR)
			return false;

		GetTransactionEntries().emplace_back(std::move(ptr));
		return true;
	}

	bool WriteCall(std::uintptr_t TargetAddress, const void *CallbackFunction, void **OriginalFunction)
	{
		// Identical to WriteJump, but with a call
		if (!TargetAddress)
			return false;

		auto ptr = std::make_unique<HookTransactionEntry>();
		ptr->TargetFunction = reinterpret_cast<void *>(TargetAddress);
		ptr->RequiresCallFixup = true;

		if (!OriginalFunction)
			OriginalFunction = &ptr->DetourStubFunction;

		*OriginalFunction = ptr->TargetFunction;

		if (DetourAttach(OriginalFunction, const_cast<void *>(CallbackFunction)) != NO_ERROR)
			return false;

		GetTransactionEntries().emplace_back(std::move(ptr));
		return true;
	}

	bool RedirectImport(const char *ImportModuleName, const char *ImportName, const void *CallbackFunction)
	{
		auto moduleCallback = [](PVOID Context, HMODULE, LPCSTR Name) -> BOOL
		{
			auto c = static_cast<IATEnumContext *>(Context);

			if (!Name || c->ModuleFound)
				return false;

			if (_stricmp(Name, c->ModuleName) == 0)
				c->ModuleFound = true;

			return true;
		};

		auto importCallback = [](PVOID Context, ULONG, PCSTR Name, PVOID *Func) -> BOOL
		{
			auto c = static_cast<const IATEnumContext *>(Context);

			if (!Func || !c->ModuleFound)
				return true;

			// If the import name matches, patch the IAT entry directly
			if (_stricmp(Name, c->ImportName) == 0)
			{
				Memory::Patch(
					reinterpret_cast<std::uintptr_t>(Func),
					reinterpret_cast<const std::uint8_t *>(&c->CallbackFunction),
					sizeof(c->CallbackFunction));

				return false;
			}

			return true;
		};

		IATEnumContext context
		{
			.ModuleName = ImportModuleName,
			.ImportName = ImportName,
			.CallbackFunction = CallbackFunction,
		};

		return DetourEnumerateImportsEx(GetModuleHandleA(nullptr), &context, moduleCallback, importCallback);
	}
}