#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "ImGuiExt.h"
#include <algorithm>
#include <cmath>

namespace ImGui
{
	void resetKeys()
	{
		for (int x = 0; x <= 255; x++)
			ImGui::GetIO().KeysDown[x] = false;
	}
	ImColor CodColor(int color, float opacity)
	{
		switch (color)
		{
		case 0:
			return ImColor(0.0f, 0.0f, 0.0f, opacity);
		case 1:
			return ImColor(1.0f, 0.0f, 0.0f, opacity);
		case 2:
			return ImColor(0.0f, 1.0f, 0.0f, opacity);
		case 3:
			return ImColor(1.0f, 1.0f, 0.0f, opacity);
		case 4:
			return ImColor(0.0f, 0.0f, 1.0f, opacity);
		case 5:
			return ImColor(0.0f, 1.0f, 1.0f, opacity);
		case 6:
			return ImColor(0.5f, 0.0f, 0.5f, opacity);
		case 7:
			return ImColor(1.0f, 1.0f, 1.0f, opacity);
		case 8:
			return ImColor(0.5f, 0.5f, 0.5f, opacity);
		case 9:
			return ImColor(0.607f, 0.607f, 0.607f, opacity);
		default:
			return ImColor(1.0f, 1.0f, 1.0f, opacity);
		}
		return ImColor(1.0f, 1.0f, 1.0f, opacity);
	}
	ImVec2 CalcCodTextSize(std::string str)
	{
		int x = str.find("^");
		while (x != std::string::npos)
		{
			if (isdigit(str.substr(x + 1, 1).c_str()[0]))
			{
				str.erase(x, 2);
				x = str.find("^", x);
			}
			else
			{
				x = str.find("^", x + 1);
			}
		}

		return ImGui::CalcTextSize(str.c_str());
	}

	float highlightFloatVal(float cFl, float intensity)
	{
		cFl += intensity;
		if (cFl > 1.0f)
			cFl = 1.0f;
		return cFl;
	}
	ImColor highlight(ImColor inColor, float intensity)
	{

		ImColor rval(highlightFloatVal(inColor.Value.x, intensity), highlightFloatVal(inColor.Value.y, intensity), highlightFloatVal(inColor.Value.z, intensity), highlightFloatVal(inColor.Value.w, intensity));
		return rval;
	}

	//Uses checkmark color for active color and text color for circle
	bool ToggleButton(const char* str_id, bool* v)
	{
		auto& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;
		return ToggleButton(str_id, v, ImVec2(0, 0), colors[ImGuiCol_Text], .15f, colors[ImGuiCol_FrameBg], colors[ImGuiCol_CheckMark], colors[ImGuiCol_Text], colors[ImGuiCol_Text]);
	}

	//Uses checkmark color for active color and text color for circle
	bool ToggleButton(const char* str_id, bool* v, ImVec2 size)
	{
		auto& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;
		return ToggleButton(str_id, v, size, colors[ImGuiCol_Text], .15f, colors[ImGuiCol_FrameBg], colors[ImGuiCol_CheckMark], colors[ImGuiCol_Text], colors[ImGuiCol_Text]);
	}


	bool ToggleButton(const char* str_id, bool* v, ImVec2 p_size, ImColor text_color, float hover_intensity, ImColor bg, ImColor bg_active, ImColor circle, ImColor circle_active)
	{
		ImGuiWindow* window = GetCurrentWindow();
		ImVec2 p = window->DC.CursorPos;
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = p_size.y == 0 ? ImGui::GetFrameHeight() : p_size.y;
		float width = p_size.x == 0 ? height * 1.55f : p_size.x;
		float radius = height * 0.50f;

		p_size.x = width;
		p_size.y = height;

		bool rval = false;
		ImVec2 text_size;
		std::string strid = str_id;

		if (strid.substr(0, 2) != "##")
			text_size = ImGui::CalcTextSize(str_id);


		if (ImGui::InvisibleButton(str_id, ImVec2(width + text_size.x, ImGui::GetFrameHeight())))
		{
			*v = !*v;
			rval = true;
		}

		ImColor col_bg;
		ImColor cir_bg;
		if (ImGui::IsItemHovered())
		{
			col_bg = *v ? highlight(bg_active, hover_intensity) : highlight(bg, hover_intensity);
			cir_bg = *v ? highlight(circle_active, hover_intensity) : highlight(circle, hover_intensity);
		}
		else
		{
			col_bg = *v ? bg_active : bg;
			cir_bg = *v ? circle_active : circle;
		}


		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
		draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, cir_bg, 24);

		if (strid.substr(0, 2) != "##")
			draw_list->AddText(ImVec2(p.x + width + 10, p.y + 3), text_color, str_id);

		return rval;
	}

	void CodText(std::string str, int wrap_width, ImVec2 fontSize, float background_opacity, float fontOpacity)
	{
		if (wrap_width > 0 && wrap_width<5000 && (str.length() * fontSize.x) > wrap_width)
		{
			int number_of_lfs = floor(((str.length() * fontSize.x) / wrap_width));
			for (int x = number_of_lfs; x > 0; x--)
			{
				int pos = floor((x * wrap_width) / fontSize.x);
				int sPos = str.find(" ");
				int lpos = std::string::npos;
				while (sPos != std::string::npos && sPos < pos)
				{
					lpos = sPos;
					sPos = str.find(" ", sPos + 2);
				}
				if (str.length() > pos)
				{
					if (lpos != std::string::npos && pos - lpos <= 20)
						pos = lpos;
					str.insert(pos, "\n");
				}


			}
		}

		std::string mStr = str;
		int current_lf = mStr.find("\n");
		if (current_lf != std::string::npos)
			mStr = str.substr(0, current_lf);
		else
			current_lf = 0;

		int color = 0;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		while (current_lf != std::string::npos)
		{
			const ImVec2 p = ImGui::GetCursorScreenPos();
			float fx = p.x - 2.0f, fy = p.y - 2.0f, spacing = 8.0f;
			ImVec2 txtSize = ImGui::CalcCodTextSize(mStr.c_str());
			draw_list->AddRectFilledMultiColor(ImVec2(fx, fy), ImVec2(fx + txtSize.x + 8.0f, fy + txtSize.y + 6.0f), ImColor(0.0f, 0.0f, 0.0f, background_opacity), ImColor(0.0f, 0.0f, 0.0f, background_opacity / 2), ImColor(0.0f, 0.0f, 0.0f, background_opacity / 2), ImColor(0.0f, 0.0f, 0.0f, background_opacity));
			//			draw_list->AddRectFilled(ImVec2(fx, fy), ImVec2(fx + txtSize.x + 8.0f, fy + txtSize.y + 6.0f), ImColor(0.0f, 0.0f, 0.0f, background_opacity));
			int colors = 0;
			int x = mStr.find("^");
			int c = 0;

			if (x != 0 && x != std::string::npos && isdigit(str.substr(x + 1, 1).c_str()[0]))
			{
				ImGui::TextColored(CodColor(7, fontOpacity), mStr.substr(0, x).c_str());
				ImGui::SameLine(0, 0);
			}
			while (x != std::string::npos)
			{

				if (isdigit(mStr.substr(x + 1, 1).c_str()[0]))
				{
					if (colors != 0)
						ImGui::SameLine(0, 0);

					colors += 1;
					c = mStr.find("^", x + 1);
					if (c != std::string::npos)
					{
						if (!isdigit(mStr.substr(c + 1, 1).c_str()[0]))
							c += 1;
						color = atoi(mStr.substr(x + 1, 1).c_str());
						std::string partial = mStr.substr(x + 2, (c - x) - 2);
						mStr.erase(x, 2);
						ImGui::TextColored(CodColor(color, fontOpacity), partial.c_str());
					}
					else
					{
						if (x >= (mStr.length() - 1))
							break;
						color = atoi(mStr.substr(x + 1, 1).c_str());
						std::string partial = mStr.substr(x + 2, (mStr.length() - (x + 2)));
						mStr.erase(x, 2);
						ImGui::TextColored(CodColor(color, fontOpacity), partial.c_str());
					}
					x = mStr.find("^", x);
				}
				else {
					c = mStr.find("^", x + 1);
					if (c != std::string::npos)
					{
						if (colors != 0)
							ImGui::SameLine(0, 0);
						std::string partial = mStr.substr(x + 1, (c - x));
						ImGui::TextColored(CodColor(color, fontOpacity), partial.c_str());
					}
					else
					{
						if (colors != 0)
							ImGui::SameLine(0, 0);
						ImGui::TextColored(CodColor(color, fontOpacity), mStr.substr(x + 1, mStr.length() - x).c_str());
					}
					x = mStr.find("^", x + 1);
				}
				if (x >= (mStr.length() - 1))
					break;

			}
			if (colors == 0 && current_lf == 0)
				ImGui::TextColored(CodColor(7, fontOpacity), mStr.c_str());
			else if (colors == 0 && current_lf > 0)
				ImGui::TextColored(CodColor(color, fontOpacity), mStr.c_str());

			if (current_lf == str.length())
				break;

			int last_lf = current_lf;
			current_lf = str.find("\n", current_lf + 2);
			if (current_lf != std::string::npos)
				mStr = str.substr(last_lf + 1, current_lf - last_lf - 1);
			else
			{
				if (last_lf != 0)
				{
					current_lf = str.length();
					mStr = str.substr(last_lf + 1, str.length() - last_lf - 1);
				}
			}
		}
	}
}