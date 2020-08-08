#pragma once
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include <string>
#include <sstream>
#include <stdint.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Singleton.hpp"

#include "../SDK/SDK.hpp"
#include "../SDK/Misc/Color.hpp"
#include "../SDK/Interfaces/ISurface.hpp"

extern unsigned long WeapIconPlayersFont;
extern unsigned long WeapIconDroppedFont;
extern unsigned long NadeIconFont;
extern unsigned long EspFont;
extern unsigned long WaterMarkFont;

class Render : public Singleton<Render>
{
	friend class Singleton<Render>;

public:
	void Initialize();
	void GetFonts();

	void RenderText(int x, int y, unsigned long font, std::string string, bool textCentered, Color color)
	{
		const auto convertedText = std::wstring(string.begin(), string.end());

		int width, height;
		g_VGuiSurface->GetTextSize(font, convertedText.c_str(), width, height);

		g_VGuiSurface->SetTextColor(color.r(), color.g(), color.b(), color.a());

		g_VGuiSurface->SetTextFont(font);

		if (textCentered)
			g_VGuiSurface->SetTextPos(x - (width / 2), y);
		else
			g_VGuiSurface->SetTextPos(x, y);

		g_VGuiSurface->RenderText(convertedText.c_str(), wcslen(convertedText.c_str()));
	}

	void RenderLine(int x1, int y1, int x2, int y2, Color color)
	{
		g_VGuiSurface->SetRenderColor(color.r(), color.g(), color.b(), color.a());

		g_VGuiSurface->RenderLine(x1, y1, x2, y2);
	}

	void RenderRect(int x, int y, int w, int h, Color color) 
	{
		g_VGuiSurface->SetRenderColor(color.r(), color.g(), color.b(), color.a());

		g_VGuiSurface->RenderOutlinedRect(x, y, w, h);
	}

	void RenderFilledRect(int x, int y, int w, int h, Color color) 
	{
		g_VGuiSurface->SetRenderColor(color.r(), color.g(), color.b(), color.a());

		g_VGuiSurface->RenderFilledRect(x, y, w, h);
	}

	void RenderOutlinedRect(int x, int y, int h, int w, Color color) 
	{
		g_VGuiSurface->SetRenderColor(color.r(), color.g(), color.b(), color.a());

		g_VGuiSurface->RenderOutlinedRect(x, y, w, h);
	}

	void RenderOutline(int x, int y, int w, int h, Color color) 
	{
		g_VGuiSurface->SetRenderColor(color.r(), color.g(), color.b(), color.a());

		g_VGuiSurface->RenderOutlinedRect(x, y, w, h);
	}
};
