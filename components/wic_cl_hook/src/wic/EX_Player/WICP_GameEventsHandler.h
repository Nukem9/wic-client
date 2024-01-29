#pragma once

class WICP_GameEventsHandler
{
public:
    enum ScriptEventCustomFunction : s32
    {
        FUNC_NONE = 0x0,
        FUNC_REQUEST = 0x1,
        FUNC_OBJECTIVE = 0x2,
        FUNC_MOVE_TO_CP = 0x3,
        FUNC_INSTANT_MESSAGE = 0x4,
    };

    bool AddScriptEvent(
        const wchar_t *aString,
        const char *anIcon,
        s32 aName,
        const char *aSound,
        const char *aGuiElementToFlash,
        const char *aPythonClientFunction,
        const char *aPythonServerFunction,
        bool myShowAsSysMessageFlag,
        ScriptEventCustomFunction aFunction,
        u32 aCustomData,
        const wchar_t *aToolTipString)
    {
        return ((bool(__stdcall *)(
            WICP_GameEventsHandler *,
            const wchar_t *aString,
            const char *anIcon,
            s32 aName,
            const char *aSound,
            const char *aGuiElementToFlash,
            const char *aPythonClientFunction,
            const char *aPythonServerFunction,
            bool myShowAsSysMessageFlag,
            ScriptEventCustomFunction aFunction,
            u32 aCustomData,
            const wchar_t *aToolTipString))0x008DBF10)(this, aString, anIcon, aName, aSound, aGuiElementToFlash, aPythonClientFunction, aPythonServerFunction, myShowAsSysMessageFlag, aFunction, aCustomData, aToolTipString);
    }
};