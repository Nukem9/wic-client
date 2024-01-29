#include "Ice.h"

const Ice *Ice::GetMember(const char *aName, FailureMode aMayFailFlag) const
{
#if WIC_MP
	return ((const Ice *(__thiscall *)(const Ice *, const char *, FailureMode))0x007864E0)(this, aName, aMayFailFlag);
#elif WIC_DS
	return ((const Ice *(__thiscall *)(const Ice *, const char *, FailureMode))0x004A2DF0)(this, aName, aMayFailFlag);
#else
#error Unimplemented
#endif
}

const char *Ice::GetValue() const
{
	auto current = this;

	while (true)
	{
		if (current->GetNumMembers() != 0)
			return nullptr;

		if (!current->IsReference())
			break;

		if (!current->myValue)
			return nullptr;

		current = current->myReference;
	}

	return current->myValue;
}

const char *Ice::GetMemberValue(const char *aName, FailureMode aMayFailFlag) const
{
	auto member = GetMember(aName, aMayFailFlag);

	if (member)
		return member->GetValue();

	return nullptr;
}

s32 Ice::GetNumMembers(bool aDelegateReferencesFlag) const
{
	auto current = this;

	while (true)
	{
		if (current->myIceType == ICE_VALUE_TYPE)
			return 0;

		if (!current->IsReference())
			break;

		if (!aDelegateReferencesFlag)
			return 0;

		if (!current->myMembers)
			return 0;

		current = myMembers;
		aDelegateReferencesFlag = true;
	}

	return current->myIceType;
}

bool Ice::IsReference() const
{
	return myIceType == ICE_REFERENCE_TYPE;
}