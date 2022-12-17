// dllmain.cpp : Defines the entry point for the DLL application.
#include "../../czApi/czApi.h"
#include "czFPSWheel.h"
namespace czFPSWheel
{
    bool czFPSWheelMenu{};
    bool czFPSWheelHud{};
    czVar* height{};
    czVar* zoom{};
    czVar* width{};
    czVar* c333{};
    czVar* c250{};
    czVar* c200{};
    czVar* c125{};
    czVar* cfps{};
    czVar* ctransfer{};
    czVar* opacity{};
    czVar* text{};
    czVar* show200{};
    czVar* transferzone{};
    dVar* var2{};
    czHook *czFPSWheelHook;
}
namespace czApi
{
    ex_czApi void getInfo(pluginInfo* pl_info)
    {
        pl_info->Author = "Human";
        pl_info->Version = "1.0";
        pl_info->Description = "Generic fps wheel";
        pl_info->UIBool = (uint64_t)&czFPSWheel::czFPSWheelMenu;
        pl_info->apiVersion = plugin_apiVersion;
        pl_info->isDependency = false;
        pl_info->priority = 10;
    }


    ex_czApi void init(HMODULE mainMod, czApi::s_czInfo* info)
    {

        czApi::czInfo = info;
        czApi::mainMod = mainMod;
        czApi::writeConsole("czFPSWheel: Enabled");
        czFPSWheel::czFPSWheelMenu = false;
        if (czApi::czInfo != nullptr)
        {
            czVarValue x;
            x.Float = 5.0f;
            czFPSWheel::height = czVar::add("fps_wheel_height", czVarType::Float, x, AutoSave_True); //this variable will auto save and load
            x.Float = 20.0f;
            czFPSWheel::zoom = czVar::add("fps_wheel_zoom", czVarType::Float, x, AutoSave_True); //this variable will auto save and load
            x.Float = 720.f;
            czFPSWheel::width = czVar::add("fps_wheel_width", czVarType::Float, x, AutoSave_True); //this variable will auto save and load

            x.Vector4 = { 0.0f, 0.0f, 1.0f, 1.0f };
            czFPSWheel::c125 = czVar::add("fps_wheel_color_125", czVarType::Vector4, x, AutoSave_True); //this variable will auto save and load
            x.Vector4 = { 1.0f, 1.0f, 1.0f, 1.0f };
            czFPSWheel::c200 = czVar::add("fps_wheel_color_200", czVarType::Vector4, x, AutoSave_True); //this variable will auto save and load
            x.Vector4 = { 0.5f, 0.0f, 0.5f, 1.0f };
            czFPSWheel::c250 = czVar::add("fps_wheel_color_250", czVarType::Vector4, x, AutoSave_True); //this variable will auto save and load
            x.Vector4 = { 1.0f, 0.0f, 0.0f, 1.0f };
            czFPSWheel::c333 = czVar::add("fps_wheel_color_333", czVarType::Vector4, x, AutoSave_True); //this variable will auto save and load
            x.Vector4 = { 0.0f, 1.0f, 0.0f, 1.0f };
            czFPSWheel::ctransfer = czVar::add("fps_wheel_color_transfer", czVarType::Vector4, x, AutoSave_True); //this variable will auto save and load
            x.Vector4 = { 1.0f, 1.0f, 1.0f, 1.0f };
            czFPSWheel::cfps = czVar::add("fps_wheel_color_text", czVarType::Vector4, x, AutoSave_True); //this variable will auto save and load
            x.Float = 1.0f;
            czFPSWheel::opacity = czVar::add("fps_wheel_opacity", czVarType::Float, x, AutoSave_True); //this variable will auto save and load
            x.Bool = true;
            czFPSWheel::text = czVar::add("fps_wheel_text", czVarType::Bool, x, AutoSave_True); //this variable will auto save and load
            czFPSWheel::show200 = czVar::add("fps_wheel_show_200", czVarType::Bool, x, AutoSave_True); //this variable will auto save and load
            x.Bool = false;
            czFPSWheel::transferzone = czVar::add("fps_wheel_transferzone_full", czVarType::Bool, x, AutoSave_True); //this variable will auto save and load
        }
    }

    ex_czApi void shutDown()
    {
        czApi::writeConsole("czFPSWheel: Disabled");
    }

}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        czApi::thisMod = hModule;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

