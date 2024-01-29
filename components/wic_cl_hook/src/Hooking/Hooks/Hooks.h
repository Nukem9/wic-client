#pragma once

#define XX_DECLARE_HOOK_INITIALIZER(name, lambda) \
	class \
	{ \
		private: \
			int myHiddenVariable = [] \
			{ \
				Hooks::AddInitializationCallback(#name, [] ##lambda); \
				return 0; \
			}(); \
	} static const ___PerFileHookCallbackDoNotUse

#if WIC_MP
#define DECLARE_HOOK_INITIALIZER_MP(name, lambda) XX_DECLARE_HOOK_INITIALIZER(name, lambda)
#define DECLARE_HOOK_INITIALIZER_DS(name, lambda)
#elif WIC_DS
#define DECLARE_HOOK_INITIALIZER_MP(name, lambda)
#define DECLARE_HOOK_INITIALIZER_DS(name, lambda) XX_DECLARE_HOOK_INITIALIZER(name, lambda)
#endif

namespace Hooks
{
	bool Initialize();
	void AddInitializationCallback(const char *Name, void(*Callback)());
	void AddInitializationCallback(const char *Name, bool(*Callback)());

	bool WriteJump(std::uintptr_t TargetAddress, const void *CallbackFunction, void **OriginalFunction = nullptr);

	template<typename T, typename U, typename... Args>
	bool WriteJump(std::uintptr_t TargetAddress, U(T::*CallbackFunction)(Args...), void **OriginalFunction = nullptr)
	{
		void *realCallbackPointer = nullptr;
		memcpy(&realCallbackPointer, &CallbackFunction, sizeof(realCallbackPointer));

		return WriteJump(TargetAddress, realCallbackPointer, OriginalFunction);
	}

	template<typename T, typename U, typename... Args>
	bool WriteJump(std::uintptr_t TargetAddress, U(T::*CallbackFunction)(Args...) const, void **OriginalFunction = nullptr)
	{
		void *realCallbackPointer = nullptr;
		memcpy(&realCallbackPointer, &CallbackFunction, sizeof(realCallbackPointer));

		return WriteJump(TargetAddress, realCallbackPointer, OriginalFunction);
	}

	template<typename T, typename U, typename... Args>
	bool WriteJump(std::uintptr_t TargetAddress, U(__fastcall T::*CallbackFunction)(Args...), void **OriginalFunction = nullptr)
	{
		void *realCallbackPointer = nullptr;
		memcpy(&realCallbackPointer, &CallbackFunction, sizeof(realCallbackPointer));

		return WriteJump(TargetAddress, realCallbackPointer, OriginalFunction);
	}

	bool WriteCall(std::uintptr_t TargetAddress, const void *CallbackFunction, void **OriginalFunction = nullptr);

	template<typename T, typename U, typename... Args>
	bool WriteCall(std::uintptr_t TargetAddress, U(__stdcall T::*CallbackFunction)(Args...), void **OriginalFunction = nullptr)
	{
		void *realCallbackPointer = nullptr;
		memcpy(&realCallbackPointer, &CallbackFunction, sizeof(realCallbackPointer));

		return WriteCall(TargetAddress, realCallbackPointer, OriginalFunction);
	}

	bool RedirectImport(const char *ImportModuleName, const char *ImportName, const void *CallbackFunction);
}