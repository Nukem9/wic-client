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
	virtual ~MN_SendFormat();
	virtual MN_ConnectionErrorType SendMe(class MN_IWriteableDataStream *aConnection, bool aDisableCompression);

	MN_SendFormat() = delete;
};

CHECK_SIZE(MN_SendFormat, 0x4);