# WIC-Client

World in Conflict client emulator files. Includes various bug fixes and protocol changes for the Massgate.org backend server. This is the client component for [WIC-MassgateServer](https://github.com/Nukem9/WIC-MassgateServer) and [massgate](https://github.com/ubisoft/massgate).

## Building

### Requirements

- Visual Studio 2019
- .NET 5 support if building the installer

### Installation

1. Build project with Visual Studio directly or run [release.ps1](release.ps1).
2. Copy output files (`wic_XX_hook.dll`, `dbghelp.dll`, `dbghelp_old.dll`) to the game folder.
3. Play.

## Patches

CL = Game, DS = Dedicated Server, BT = Broadcast Tool, ED = Modkit Editor

- DNS queries redirected to `liveaccount.massgate.org`. [CL, DS, BT, ED]
- Multiple threading fixes, such as crashing when using a CPU with more than 16 logical cores. [CL, DS, BT, ED]
- Multiple command line options have been added ("-fps", "-ignorealttab", "-nocursorspeed"). [CL, DS]
- Network protocol has been updated to skip pointless CD-key checks. [CL, BT, DS]
- Fix a use-after-free where the game incorrectly holds a pointer to a MC_Str<> after it has exited scope. [CL, BT]
- Fix a bug where mods were loaded synchronously in a GUI handler when they should've been using the app event queue. [CL, BT]
- Fix an out of bounds access when files without an extension are present in the game or mod folders. [CL, BT]
- Fix a crash from a null pointer dereference when EX_CAI_CommanderAI_Data::CompareEnemyGroups has no groups available. [CL]
- Fix for the rounding error in team tick rates when playing domination. [CL, DS]
- Fix for crash when cPlayer_ChatMessage would use an incorrect player slot to send chat messages. [DS]
- Fix for floating point exceptions being raised and causing crashes. [DS, ED]
- Numerous assertions removed to prevent crashes and to allow Modern Warfare to work correctly. [DS]
- Servers are now allowed to use mods in ranked mode. [DS]
- Servers now search for the "Mods" directory in the local folder instead of "C:\Users\Public Documents\". [DS]

## License

- [LGPL](LICENSE.md)
- [detours](components/shared/detours) and [TBB](components/shared/tbb) are under their own respective licenses.

## Authors

- [Nukem9](https://github.com/Nukem9)
- [Tenerefis](https://github.com/tenerefis)
