#include "czBind.h"
#define GS_gameFocused		(*(bool*)0x0CC1B704)
#define GS_needsInput 		(*(DWORD*)0xC5F8F8!=0)
#define GS_connectionState	(*(int*)0xC5F900)
#define GS_isConnected		(GS_connectionState == 9)
czBind::czBind(std::string name, std::string desc, int default_key, int default_modifier, int default_WM)
{
    state = bindState::up;
    czVarValue x;
    x.Vector3.x = default_key;
    x.Vector3.y = default_modifier;
    x.Vector3.z = default_WM;
    bindVar = czVar::add("bind_" + name, czVarType::Vector3, x, true, desc);
}

bool czBind::asyncKeyDown()
{
    if (!GS_gameFocused || (GS_isConnected && GS_needsInput))
        return false;
    float modifier = 0;
    if (GetAsyncKeyState(VK_MENU))
        modifier += modifierKeys::alt;
    if (GetAsyncKeyState(VK_SHIFT))
        modifier += modifierKeys::shift;
    if (GetAsyncKeyState(VK_CONTROL))
        modifier += modifierKeys::control;
    if (modifier != (int)bindVar->value.Vector3.y)
        return false;
    return GetAsyncKeyState((int)bindVar->value.Vector3.x) & 0x8000;

}
void czBind::update(UINT_PTR Keycode, UINT WindowsMessage)
{
    float modifier = 0;
    if (GetAsyncKeyState(VK_MENU))
        modifier += modifierKeys::alt;
    if (GetAsyncKeyState(VK_SHIFT))
        modifier += modifierKeys::shift;
    if (GetAsyncKeyState(VK_CONTROL))
        modifier += modifierKeys::control;

    if (state == bindState::pressed)
        state = bindState::up;

    if (tDown.Elapsed() > 250 && state == bindState::down)
        state = bindState::held;





    /*if ((state == bindState::down || state==bindState::held) && (WindowsMessage == WM_LBUTTONUP || WindowsMessage == WM_RBUTTONUP || WindowsMessage == WM_MBUTTONUP || WindowsMessage == WM_XBUTTONUP))
    {
        if (Keycode < 0x10 || Keycode>0xFE)
        {
            if (state == bindState::down && tDown.Elapsed() < 250)
                state = bindState::pressed;
            else
                state = bindState::up;
        }
    }*/

    if (modifier == (int)bindVar->value.Vector3.y && Keycode == (UINT_PTR)bindVar->value.Vector3.x && WindowsMessage == (int)bindVar->value.Vector3.z)
    {
            if (state != bindState::down && state != bindState::held && WindowsMessage != WM_MOUSEWHEEL)
            {
                state = bindState::down;
                tDown.Reset();
            }
            else if (WindowsMessage == WM_MOUSEWHEEL)
            {
                state = bindState::pressed;
            }

    }
    else
    if (WindowsMessage == WM_KEYUP || WindowsMessage == WM_SYSKEYUP || WindowsMessage == WM_LBUTTONUP || WindowsMessage == WM_RBUTTONUP || WindowsMessage == WM_MBUTTONUP || WindowsMessage == WM_XBUTTONUP || WindowsMessage == WM_MOUSEWHEEL)
    {
        if (WindowsMessage == WM_KEYUP || WindowsMessage == WM_SYSKEYUP)
        {
            if (Keycode == (UINT_PTR)bindVar->value.Vector3.x && (WindowsMessage == (int)((bindVar->value.Vector3.z) + 1)))
            {
                if (state == bindState::down && tDown.Elapsed() < 250)
                    state = bindState::pressed;
                else
                    state = bindState::up;
            }
        }
        else
        {
            if (WindowsMessage == (int)((bindVar->value.Vector3.z) + 1))
            {
                if (state == bindState::down && tDown.Elapsed() < 250)
                    state = bindState::pressed;
                else
                    state = bindState::up;
            }
        }
    }
    //std::stringstream ss;
    //ss << "Modifier: " << modifier << " Keycode: " << Keycode;
    //ss << " Needed Mod: " << bindVar->value.Vector3.y << " Needed Keycode: " << (int)bindVar->value.Vector3.x << " WM: " << WindowsMessage << " Needed: " << (int)bindVar->value.Vector3.z << " State: " << (int)state << std::endl;
    //czApi::writeConsole(ss.str());
}
