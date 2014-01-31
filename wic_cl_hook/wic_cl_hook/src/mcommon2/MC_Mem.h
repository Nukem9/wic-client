#pragma once

#define MC_ASSERT(condition) if (!(condition)) MC_Assert(__FILE__, __LINE__, #condition, nullptr)

void MC_Assert(const char *aFile, int aLine, const char *aString, bool *aIgnoreFlag);