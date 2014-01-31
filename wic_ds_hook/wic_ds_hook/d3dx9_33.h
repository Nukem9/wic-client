#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

extern "C" __declspec(dllexport) HRESULT D3DXAssembleShader(LPCSTR pSrcData, UINT SrcDataLen, const PVOID *pDefines, PVOID pInclude, DWORD Flags, PVOID *ppShader, PVOID *ppErrorMsgs);
extern "C" __declspec(dllexport) HRESULT D3DXCompileShader(LPCSTR pSrcData, UINT srcDataLen, const PVOID *pDefines, PVOID pInclude, LPCSTR pFunctionName, LPCSTR pProfile, DWORD Flags, PVOID *ppShader, PVOID *ppErrorMsgs, PVOID *ppConstantTable);
extern "C" __declspec(dllexport) HRESULT D3DXCreateBuffer(DWORD NumBytes, PVOID *ppBuffer);