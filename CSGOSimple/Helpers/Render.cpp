#include "Render.hpp"
#include "InputSys.hpp"

#include "../SDK/SDK.hpp"
#include "../Menu/Menu.hpp"
#include "../Fonts/Fonts.hpp"
#include "../SDK/Math/Math.hpp"
#include "../SDK/CSGOStructs.hpp"

unsigned long WeapIconPlayersFont;
unsigned long WeapIconDroppedFont;
unsigned long NadeIconFont;
unsigned long EspFont;
unsigned long WaterMarkFont;

void Render::Initialize() 
{
	GetFonts();
}

void Render::GetFonts() {
	WeapIconPlayersFont = g_VGuiSurface->FontCreate();
	WeapIconDroppedFont = g_VGuiSurface->FontCreate();
	NadeIconFont = g_VGuiSurface->FontCreate();
	EspFont = g_VGuiSurface->FontCreate();
	WaterMarkFont = g_VGuiSurface->FontCreate();

	g_VGuiSurface->SetFontGlyph(WeapIconPlayersFont, "AstriumWep", 10, 400, 0, 0, FontFlags::FONTFLAG_ANTIALIAS | FontFlags::FONTFLAG_DROPSHADOW);
	g_VGuiSurface->SetFontGlyph(WeapIconDroppedFont, "AstriumWep", 12, 400, 0, 0, FontFlags::FONTFLAG_ANTIALIAS | FontFlags::FONTFLAG_DROPSHADOW);
	g_VGuiSurface->SetFontGlyph(NadeIconFont, "AstriumWep", 20, 400, 0, 0, FontFlags::FONTFLAG_ANTIALIAS);
	g_VGuiSurface->SetFontGlyph(EspFont, "Verdana Bold", 12, 500, 0, 0, FontFlags::FONTFLAG_ANTIALIAS | FontFlags::FONTFLAG_DROPSHADOW);
	g_VGuiSurface->SetFontGlyph(WaterMarkFont, "Tahoma", 12, 500, 0, 0, FontFlags::FONTFLAG_DROPSHADOW);
}