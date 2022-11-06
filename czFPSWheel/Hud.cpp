#include "Hud.h"

//bool HudElement(std::string title, bool force_title)
//{
//    ImGui::SetNextWindowPos(ImVec2(500, 500), ImGuiCond_FirstUseEver);
//    ImGui::SetNextWindowSize(ImVec2(785, 265), ImGuiCond_FirstUseEver);
//    static float titlebar_height;
//    if (exampleMenu || force_title)
//    {
//        if (!ImGui::Begin(title.c_str(), &exampleHud, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
//        {
//            ImGui::End();
//            return false;
//        }
//    }
//    else {
//        if (!ImGui::Begin(title.c_str(), &exampleHud, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration))
//        {
//            ImGui::End();
//            return false;
//        }
//    }
//    return true;
//}
//
//void hudTime()
//{
//
//    ImGui::SetNextWindowBgAlpha(hud::opacity);
//    bool open_popup = false;
//    if (exampleHud && HudElement("Hud Time", false))
//    {
//        ImGui::SetWindowFontScale(hud::fontSize);
//        char* buffer = new char[250];
//        time_t now = time(NULL);
//        struct tm buf;
//        localtime_s(&buf, &now);
//        strftime(buffer, 250, "%I:%M:%S", &buf);
//
//       ImGui::TextColored(hud::tColor, "%s", buffer);
//        buffer = NULL;
//        if (ImGui::GetIO().MouseClicked[1] && ImGui::IsWindowHovered())
//            open_popup = true;
//        ImGui::End();
//        if (open_popup)
//            ImGui::OpenPopup("timePopup");
//    }
//    if (ImGui::BeginPopup("timePopup"))
//    {
//        ImGui::SliderFloat("Font Size", &hud::fontSize, 0, 8.0, "%f", 1.0f);
//        ImGui::SliderFloat("Background Opacity", &hud::opacity, 0, 1.0, "%f", 1.0f);
//        ImGui::ColorEdit4("Font Color", (float*)&hud::tColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview);
//        ImGui::EndPopup();
//    }
//
//}