#include "../../czApi/czApi.h"
#include "czFPSWheel.h"

ex_czApi bool czApi::keyboardEvent(WPARAM Keycode, UINT WindowsMessage) //return true if handled (blocked input from going further), false if not handled and needs passed on
{
    static Timer noSpam;
    if (noSpam.Elapsed() > 50 && WindowsMessage == WM_KEYDOWN)
    {
        //if (static_cast<UINT_PTR>(Keycode) == VK_END)
        //{
        //    czFPSWheel::czFPSWheelMenu = !czFPSWheel::czFPSWheelMenu;
        //    return true; //return true this key press doesn't go any further
        //}
        noSpam.Reset();
    }
    return false;
}
