#pragma once

class IceLocDecoder;

#define ICE_MAX_STRING 1048576
#define ICE_VALUE_TYPE -1
#define ICE_REFERENCE_TYPE -2

class Ice
{
public:
	enum FailureMode : s32
	{
		AssertOnFail,
		WarnOnFail,
		DontWarnOnFail,
	};

	class ReturnProxy
	{
		const Ice *myIce;
	};
	static_assert_size(ReturnProxy, 0x4);

private:
	Ice *myParent;
	IceLocDecoder *myLocDecoder;

	union
	{
		char *myValue;
		const Ice *myReference;
		Ice *myMembers;
	};

	s32 myTypeName;
	s32 myInstanceName;
	s32 myIceType;
	char *myDataBuf;
	char *myName;

public:
	const Ice *GetMember(const char *aName, FailureMode aMayFailFlag = FailureMode::WarnOnFail) const;
	const char *GetValue() const;
	const char *GetMemberValue(const char *aName, FailureMode aMayFailFlag = FailureMode::WarnOnFail) const;
	s32 GetNumMembers(bool aDelegateReferencesFlag = false) const;
	bool IsReference() const;
};
static_assert_size(Ice, 0x20);