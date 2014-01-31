#pragma once

enum MN_ConnectionErrorType
{
	MN_CONN_OK,
	MN_CONN_NODATA,
	MN_CONN_BROKEN,
};

class MN_SendFormat
{
public:
	virtual void *__vecDelDtor(unsigned int);

	virtual MN_ConnectionErrorType SendMe(void /*MN_IWriteableDataStream*/ *aConnection, bool aDisableCompression);

	//MN_SendFormat();
};

CHECK_SIZE(MN_SendFormat, 0x4);