#include "d3dx9_33.h"

extern "C" __declspec(dllexport) HRESULT D3DXAssembleShader(LPCSTR pSrcData, UINT SrcDataLen, const PVOID *pDefines, PVOID pInclude, DWORD Flags, PVOID *ppShader, PVOID *ppErrorMsgs)
{
	return S_OK;
}

extern "C" __declspec(dllexport) HRESULT D3DXCompileShader(LPCSTR pSrcData, UINT srcDataLen, const PVOID *pDefines, PVOID pInclude, LPCSTR pFunctionName, LPCSTR pProfile, DWORD Flags, PVOID *ppShader, PVOID *ppErrorMsgs, PVOID *ppConstantTable)
{
	return S_OK;
}

extern "C" __declspec(dllexport) HRESULT D3DXCreateBuffer(DWORD NumBytes, PVOID *ppBuffer)
{
	return S_OK;
}