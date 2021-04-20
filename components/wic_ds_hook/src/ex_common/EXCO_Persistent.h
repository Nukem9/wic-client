#pragma once

#pragma pack(push, 1)
class EXCO_PersistentBase // Inherited class at offset 0x0
{
public:
	virtual void *__vecDelDtor(unsigned int);
	virtual const char *GetTypeName();
	//virtual bool PersistDynamicMembers(EXCO_PersistenceDatabaseWriteLump *);
	//virtual bool CreateDynamicMember(EXCO_PersistentDynamicMember *);

	MC_GrowingArray<EXCO_PersistentMember *>	myMembers;		// this+0x4
	const uint32_t								myKey;			// this+0x14
	EXCO_PersistentBase							*myNextInstance;// this+0x18
};

template<typename T>
class EXCO_Persistent : public EXCO_PersistentBase
{
public:
};

CHECK_SIZE(EXCO_PersistentBase, 0x1C);
#pragma pack(pop)