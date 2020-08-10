#pragma once

#include <d3d9.h>
#include <string>

#include "../SDK/SDK.hpp"
#include "../Helpers/Configs.hpp"
#include "../Helpers/InputSys.hpp"

#include "ImGui/imgui.h"
#include "ImGui/dx9/imgui_impl_dx9.h"

extern IDirect3DStateBlock9* stateBlock;

class CMenu
{
public:
	void Run();
	void RunPopup();

	void __stdcall CreateObjects(IDirect3DDevice9* device)
	{
		if (InputSys::Get().m_hTargetWindow)
			ImGui_ImplDX9_CreateDeviceObjects();
	}

	void __stdcall InvalidateObjects()
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void __stdcall SetupEndScene(IDirect3DDevice9* device)
	{
		ImGui_ImplDX9_Init(InputSys::Get().m_hTargetWindow, device);

		ImGuiStyle& Style = ImGui::GetStyle();
		Style.Alpha = 1.0f;
		Style.WindowPadding = ImVec2(0, 0);
		Style.WindowMinSize = ImVec2(32, 32);
		Style.WindowRounding = 0.0f;
		Style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		Style.ChildWindowRounding = 0.0f;
		Style.FramePadding = ImVec2(4, 3);
		Style.FrameRounding = 0.0f;
		Style.ItemSpacing = ImVec2(8, 8);
		Style.ItemInnerSpacing = ImVec2(8, 8);
		Style.TouchExtraPadding = ImVec2(0, 0);
		Style.IndentSpacing = 21.0f;
		Style.ColumnsMinSpacing = 0.0f;
		Style.ScrollbarSize = 6.0f;
		Style.ScrollbarRounding = 0.0f;
		Style.GrabMinSize = 5.0f;
		Style.GrabRounding = 0.0f;
		Style.ButtonTextAlign = ImVec2(0.0f, 0.5f);
		Style.DisplayWindowPadding = ImVec2(22, 22);
		Style.DisplaySafeAreaPadding = ImVec2(4, 4);
		Style.AntiAliasedLines = true;
		Style.AntiAliasedShapes = false;
		Style.CurveTessellationTol = 1.f;

		ImVec4* Colors = ImGui::GetStyle().Colors;
		Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		Colors[ImGuiCol_WindowBg] = ImVec4(33 / 255.f, 35 / 255.f, 47 / 255.f, 1.0f);
		Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		Colors[ImGuiCol_Border] = ImVec4(30 / 255.f, 30 / 255.f, 41 / 255.f, 1.0f);
		Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		Colors[ImGuiCol_ChildWindowBg] = ImVec4(33 / 255.f, 35 / 255.f, 47 / 255.f, 1.0f);
		Colors[ImGuiCol_FrameBg] = ImVec4(33 / 255.f, 35 / 255.f, 47 / 255.f, 1.0f);
		Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		Colors[ImGuiCol_FrameBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		Colors[ImGuiCol_TitleBgActive] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
		Colors[ImGuiCol_TitleBg] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
		Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
		Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		Colors[ImGuiCol_Button] = ImVec4(33 / 255.f, 35 / 255.f, 47 / 255.f, 1.0f);
		Colors[ImGuiCol_ButtonHovered] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
		Colors[ImGuiCol_ButtonActive] = ImVec4(135 / 255.f, 135 / 255.f, 135 / 255.f, 1.0f); 
		Colors[ImGuiCol_Header] = ImVec4(0.654f, 0.094f, 0.278f, 1.f);
		Colors[ImGuiCol_HeaderHovered] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
		Colors[ImGuiCol_HeaderActive] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
		Colors[ImGuiCol_Separator] = ImVec4(0, 0, 0, 1);
		Colors[ImGuiCol_SeparatorHovered] = ImVec4(0, 0, 0, 1);
		Colors[ImGuiCol_SeparatorActive] = ImVec4(0, 0, 0, 1);
		Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		Colors[ImGuiCol_CloseButton] = ImVec4(0, 0, 0, 0);
		Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0, 0, 0, 0);
		Colors[ImGuiCol_HotkeyOutline] = ImVec4(0, 0, 0, 0);

		CreateObjects(device);
	}

	void ApplyFonts()
	{
		ImGui::CreateContext();

		FontMain = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 18);
		FontMenu = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 12);
	}

	void __stdcall EndEndScene(IDirect3DDevice9* device)
	{
		if (Opened)
		{
			if (ImGui::GetStyle().Alpha > 1.f)
				ImGui::GetStyle().Alpha = 1.f;

			else if (ImGui::GetStyle().Alpha != 1.f)
				ImGui::GetStyle().Alpha += 0.05f;
		}

		ImGui::Render();

		stateBlock->Apply();
		stateBlock->Release();
	}

	void __stdcall PreRender(IDirect3DDevice9* device)
	{
		D3DVIEWPORT9 D3DViewPort;
		device->GetViewport(&D3DViewPort);

		device->CreateStateBlock(D3DSBT_ALL, &stateBlock);
		stateBlock->Capture();

		device->SetVertexShader(nullptr);
		device->SetPixelShader(nullptr);
		device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

		device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	}

	void __stdcall PostRender()
	{
		ImGui_ImplDX9_NewFrame();
	}

	ImFont* FontMain;
	ImFont* FontMenu;
	bool Opened = true;
};

extern CMenu Menu;