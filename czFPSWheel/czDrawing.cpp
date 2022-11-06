#include "../../czApi/czApi.h"
#include "czFPSWheel.h"
#include <map>
#define isSpectating (czGame::cg->clientNum != czGame::cg->spectating_clientNum)
struct FPSWheelItem
{
    std::string caption;
    Vec4 color;
    float size; // The assumption is that all the fps starts at 125 and ends at 333 in a 90 degree section so all we need to have is the size of each one
    FPSWheelItem()
    {

    }
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


void draw250_speedup(int r, int rot, float zoom, Vec2 mid, Vec2 size)
{
    ImColor color;
    color.Value.x = 0.5f;
    color.Value.y = 0.5f;
    color.Value.z = 0.5f;
    color.Value.w = 1.0f;
    //Vec4 cwishDir = czMove::getWishDir(0, GS_CMD);

    float offset = rot==fps_rot_left? 94 : 86;
    float nanglediff = czMath::NormalizeToRange(getViewAngle() - offset + (90 * r), -180, 180);
    ImGui::GetWindowDrawList()->AddLine({ mid.x + (nanglediff * zoom), mid.y }, { mid.x + (nanglediff * zoom), mid.y + size.y }, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 8);
}



void AngleVectors(const Vec3 angles, Vec3* forward, Vec3* right, Vec3* up) {
    float angle;
    static float sr, sp, sy, cr, cp, cy, cpi = (M_PI * 2 / 360);

    angle = angles[1] * cpi;
    sy = sin(angle);
    cy = cos(angle);
    angle = angles[0] * cpi;
    sp = sin(angle);
    cp = cos(angle);
    angle = angles[2] * cpi;
    sr = sin(angle);
    cr = cos(angle);

    if (forward) {
        forward->x = cp * cy;
        forward->y = cp * sy;
        forward->z = -sp;
    }

    if (right) {
        right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
        right->y = (-1 * sr * sp * sy + -1 * cr * cy);
        right->z = -1 * sr * cp;
    }

    if (up) {
        up->x = (cr * sp * cy + -sr * -sy);
        up->y = (cr * sp * sy + -sr * cy);
        up->z = cr * cp;
    }
}

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
    fps_rot_ cRot = fps_rot_left;
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

ex_czApi void czApi::draw(LPDIRECT3DDEVICE9 dev, DWORD* imgui_context)
{

    if (GImGui == nullptr)
    {
        //All of the sizes added together should be equal to 90 degrees
        FPSVec["125"] = FPSWheelItem("125", czFPSWheel::c125->Value.Vector4, 13.3f);
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
    for (int i = 0; i <= 4; i++)
    {
        int rotation = (i * 90);
        float start_pos = 0;
        for (auto& [val, fps] : FPSVec)
        {
            float fShift = cRot == fps_rot_right ? -rot_shift : rot_shift;
            float anglediff = normalize(getViewAngle() - fShift + rotation);
            //Translation to the window
            Vec2 TopLeft;
            Vec2 BottomRight;
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
            float cMid = TopLeft.x + ((BottomRight.x - TopLeft.x) / 2);
            if (czFPSWheel::text->Value.Bool)
            ImGui::GetWindowDrawList()->AddText({ cMid, TopLeft.y + height }, ImColor(fps.color.x, fps.color.y, fps.color.z, opacity), fps.caption.c_str());
            ImGui::GetWindowDrawList()->AddRectFilled(TopLeft.toImVec2(), BottomRight.toImVec2(), ImColor(fps.color.x,fps.color.y,fps.color.z, opacity), 0);
            start_pos += (fps.size*zoom);
        }
    }

    for (int i = 0; i < 4; i++)
        draw250_speedup(i, cRot, zoom, mid, { 0, height });


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
        ImGui::Checkbox("DisplayText", &czFPSWheel::text->Value.Bool);
        if (ImGui::GetIO().KeysDown[VK_ESCAPE])
        {
            ImGui::GetIO().KeysDown[VK_ESCAPE] = false;
            czFPSWheel::czFPSWheelMenu = false;
        }
        ImGui::End();
    }
}
