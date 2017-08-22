#pragma once

bool RelaunchWicExecutable(wchar_t *NewExecutable, wchar_t *OldExecutable);
bool GetModuleVersion(wchar_t *Path, int *Major, int *Minor, int *Build, int *Revision);