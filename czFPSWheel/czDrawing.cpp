#include "../../czApi/czApi.h"
#include "czFPSWheel.h"
#include <map>
#define isSpectating (czGame::cg->clientNum != czGame::cg->spectating_clientNum)
struct FPSWheelItem
{
    std::string caption;
    Vec4 color;
    float size; // The assumption is that all the fps starts at 125 and ends at 333 in a 90 degree section so all we need to have is the size of each one
	FPSWheelItem() : caption(""), color(0, 0, 0, 0), size(0) {}
    FPSWheelItem(std::string sCaption, Vec4 cColor, float fSize)
    {
        caption = sCaption;
        color = cColor;
        size = fSize;
    }
};

float rot_shift = 29.0f; 
std::unordered_map<std::string,FPSWheelItem> FPSVec;

float getViewAngle()
{
    float cView = czGame::refDef->ViewAngles.y;
    if (GS_isSpectating)
        return czGame::refDef->ViewAngles.y;

    if (GS_CMD->strafe != strafe_none && GS_CMD->forward == strafe_none)
    {
        if (GS_CMD->strafe == strafe_left)
            cView += 45.0f;
        else
            cView -= 45.0f;
    }
    return cView;
}

float normalize(float val)
{
    return fmodf(val + 180 + 360, 360) - 180;
}
float angleDiff(float angle1, float angle2)
{
    return fmodf((angle1 - angle2) + 360 + 180, 360) - 180;
}
enum fps_rot_ : char
{
    fps_rot_left,
    fps_rot_right
};


struct fps_zone
{
    int fps;
    float start;
    float end;
    fps_zone(int iFPS, float fStart, float fEnd)
    {
        fps = iFPS;
        start = fStart;
        end = fEnd;
    }
    fps_zone()
    {
    }
};


fps_rot_ get_rotation()
{
   static fps_rot_ cRot = fps_rot_left;
    if (GS_CMD->strafe == strafe_left)
        cRot = fps_rot_right;
    else if (GS_CMD->strafe == strafe_right)
        cRot = fps_rot_left;

    static float previous_rotation = 0;
    static float previous_view_angle = getViewAngle();
    float current_rotation = angleDiff(getViewAngle(), previous_view_angle);
    if (fabs(current_rotation) > 3.0f)
    {
        current_rotation = angleDiff(getViewAngle(), previous_view_angle);
        previous_view_angle = getViewAngle();
    }
    previous_rotation = current_rotation;
    
    if (GS_isSpectating || !GS_CMD->strafe)
    {
            if (current_rotation > 0)
                cRot = fps_rot_right;
            else if (current_rotation < 0)
                cRot = fps_rot_left;
    }
    return cRot;
}

void get_rot_rect(Vec2 &TopLeft, Vec2 &BottomRight, float rotation, float start_pos, float zoom, float height, FPSWheelItem& fps)
{
    fps_rot_ cRot = get_rotation();
    Vec2 mid = { ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2), ImGui::GetWindowPos().y };
    float fShift = cRot == fps_rot_right ? -rot_shift : rot_shift;
    float anglediff = normalize(getViewAngle() - fShift + rotation);
    
    if (cRot == fps_rot_left)
    {
        TopLeft = { mid.x + start_pos + (anglediff * zoom), mid.y };
        BottomRight = { TopLeft.x + (fps.size * zoom), mid.y + height };
    }
    else
    {
        TopLeft = { mid.x - start_pos + (anglediff * zoom), mid.y };
        BottomRight = { TopLeft.x - (fps.size * zoom), mid.y + height };
    }
}


ex_czApi void czApi::draw(LPDIRECT3DDEVICE9 dev, DWORD* imgui_context)
{

    if (GImGui == nullptr)
    {
        //All of the sizes added together should be equal to 90 degrees
        FPSVec["125"] = FPSWheelItem("125", czFPSWheel::c125->Value.Vector4, czFPSWheel::show200->Value.Bool ? 13.3f : 22.7f);
		if (czFPSWheel::show200->Value.Bool)
            FPSVec["200"] = FPSWheelItem("200", czFPSWheel::c200->Value.Vector4, 9.4f);
        FPSVec["250"] = FPSWheelItem("250", czFPSWheel::c250->Value.Vector4, 20.35f);
        FPSVec["333"] = FPSWheelItem("333", czFPSWheel::c333->Value.Vector4, 46.95f);
        GImGui = (ImGuiContext*)imgui_context;
    }


    if (czFPSWheel::height == nullptr || !GS_isConnected)
        return;
    float height = czFPSWheel::height->Value.Float;
    ImGui::SetNextWindowPos(ImVec2(600,600), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(czFPSWheel::width->Value.Float, height+60), ImGuiCond_Always);

    if (!ImGui::Begin("FPS Wheel", &czFPSWheel::czFPSWheelHud, ImGuiWindowFlags_HudNoInput | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    Vec2 mid = { ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2), ImGui::GetWindowPos().y };
    
    float zoom = czFPSWheel::zoom->Value.Float;
    float opacity = czFPSWheel::opacity->Value.Float;
    fps_rot_ cRot = get_rotation();
    rot_shift = -4.f;// *(ImGui::GetWindowSize().x / 720);
    for (int i = 0; i < 4; i++)
    {
        int rotation = (i * 90);
        float start_pos = 0;
        for (auto& [val, fps] : FPSVec)
        {
            Vec2 TopLeft, BottomRight;
            get_rot_rect(TopLeft, BottomRight, rotation, start_pos, zoom, height, fps);
            float cMid = TopLeft.x + ((BottomRight.x - TopLeft.x) / 2);

    
            ImGui::GetWindowDrawList()->AddRectFilled(TopLeft.toImVec2(), BottomRight.toImVec2(), ImColor(fps.color.x,fps.color.y,fps.color.z, opacity), 0);
            
            if (val == "250") //transfer zone
            {
                FPSWheelItem x = FPSWheelItem("", Vec4(0.0f, 1.0f, 0.0f, 0.3f), FPSVec["250"].size / 2.4f);
                get_rot_rect(TopLeft, BottomRight, rotation, start_pos + ((FPSVec["250"].size*zoom) / 1.7), zoom, height, x);
				
                if (czFPSWheel::transferzone->Value.Bool)
                    ImGui::GetWindowDrawList()->AddRectFilled(TopLeft.toImVec2(), BottomRight.toImVec2(), czFPSWheel::ctransfer->Value.Vector4.toImColor(), 0);
                else
                    ImGui::GetWindowDrawList()->AddLine(TopLeft.toImVec2(), Vec2(TopLeft.x, TopLeft.y+height).toImVec2(), czFPSWheel::ctransfer->Value.Vector4.toImColor(), 6);
            }
			

            if (czFPSWheel::text->Value.Bool)
                ImGui::GetWindowDrawList()->AddText(nullptr, height, { cRot == fps_rot_left ? cMid - 20 : cMid, TopLeft.y }, czFPSWheel::cfps->Value.Vector4.toImColor(), fps.caption.c_str());

            start_pos += (fps.size*zoom);
        }
    }

    ImGui::GetWindowDrawList()->AddLine({ mid.x, mid.y }, { mid.x, mid.y + height }, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
    ImGui::End();

    if (czFPSWheel::czFPSWheelMenu)
    {
        ImGui::SetNextWindowPos(ImVec2(600, 600), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(720,150), ImGuiCond_FirstUseEver);
        ImGui::Begin(ICON_FA_COGS" FPS Wheel", &czFPSWheel::czFPSWheelMenu);
        ImGui::SliderFloat("Height", &czFPSWheel::height->Value.Float, 0, 100);
        ImGui::SliderFloat("Width", &czFPSWheel::width->Value.Float, 100, 2048);
        ImGui::SliderFloat("Zoom", &czFPSWheel::zoom->Value.Float, 0, 100);
        ImGui::SliderFloat("Opacity", &czFPSWheel::opacity->Value.Float, 0, 1);
        ImGui::Spacing();
        ImGui::Separator();
        if (ImGui::ColorEdit4("333", (float*)&czFPSWheel::c333->Value.Vector4, ImGuiColorEditFlags_NoInputs))
        {
            FPSVec["333"].color = czFPSWheel::c333->Value.Vector4;
        }
        if (ImGui::ColorEdit4("250", (float*)&czFPSWheel::c250->Value.Vector4, ImGuiColorEditFlags_NoInputs))
        {
            FPSVec["250"].color = czFPSWheel::c250->Value.Vector4;
        }
        if (ImGui::ColorEdit4("200", (float*)&czFPSWheel::c200->Value.Vector4, ImGuiColorEditFlags_NoInputs))
        {
            FPSVec["200"].color = czFPSWheel::c200->Value.Vector4;
        }
        if (ImGui::ColorEdit4("125", (float*)&czFPSWheel::c125->Value.Vector4, ImGuiColorEditFlags_NoInputs))
        {
            FPSVec["125"].color = czFPSWheel::c125->Value.Vector4;
        }
        ImGui::ColorEdit4("Transfer", (float*)&czFPSWheel::ctransfer->Value.Vector4, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Text Color", (float*)&czFPSWheel::cfps->Value.Vector4, ImGuiColorEditFlags_NoInputs);
        ImGui::Checkbox("DisplayText", &czFPSWheel::text->Value.Bool);
        if (ImGui::Checkbox("Show 200", &czFPSWheel::show200->Value.Bool))
        {
            if (!czFPSWheel::show200->Value.Bool)
            {
                FPSVec.erase("200");
                FPSVec["125"].size=22.7f;
            }
            else
            {
                FPSVec["125"].size= 13.3f;
                FPSVec["200"] = FPSWheelItem("200", czFPSWheel::c200->Value.Vector4, 9.4f);
            }
        }
			
        ImGui::Checkbox("Fill transfer zone", &czFPSWheel::transferzone->Value.Bool);
        if (ImGui::GetIO().KeysDown[VK_ESCAPE])
        {
            ImGui::GetIO().KeysDown[VK_ESCAPE] = false;
            czFPSWheel::czFPSWheelMenu = false;
        }
        ImGui::End();
    }
}
