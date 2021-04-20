#pragma once

#pragma pack(push, 1)
class EXCO_PersistentMember 
{
	virtual void *__vecDelDtor(uint32_t);
	virtual bool PreSaveValidatePointer(const char *, const uint32_t);
	virtual bool PostLoadValidatePointer(const char *, const uint32_t);
	//virtual bool Persist(MF_File *);
	//virtual void Read(EXCO_PersistenceDatabaseReadLump *);
	//virtual void ResolvePointersTo(void *, const char *, const unsigned int);

	const MC_Str<char, 0> myName;// this+0x4
};

CHECK_SIZE(EXCO_PersistentMember, 0x8);
#pragma pack(pop)