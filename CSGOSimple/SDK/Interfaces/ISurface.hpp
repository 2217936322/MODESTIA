#pragma once

#include "IAppSystem.hpp"
#include "../Math/Vector2D.hpp"

namespace vgui
{
    typedef unsigned long HFont;
    typedef unsigned int VPANEL;
};

enum FontFeature
{
    FONT_FEATURE_ANTIALIASED_FONTS = 1,
    FONT_FEATURE_DROPSHADOW_FONTS = 2,
    FONT_FEATURE_OUTLINE_FONTS = 6,
};

enum FontDrawType
{
    FONT_DRAW_DEFAULT = 0,
    FONT_DRAW_NONADDITIVE,
    FONT_DRAW_ADDITIVE,
    FONT_DRAW_TYPE_COUNT = 2,
};

enum FontFlags
{
    FONTFLAG_NONE,
    FONTFLAG_ITALIC = 0x001,
    FONTFLAG_UNDERLINE = 0x002,
    FONTFLAG_STRIKEOUT = 0x004,
    FONTFLAG_SYMBOL = 0x008,
    FONTFLAG_ANTIALIAS = 0x010,
    FONTFLAG_GAUSSIANBLUR = 0x020,
    FONTFLAG_ROTARY = 0x040,
    FONTFLAG_DROPSHADOW = 0x080,
    FONTFLAG_ADDITIVE = 0x100,
    FONTFLAG_OUTLINE = 0x200,
    FONTFLAG_CUSTOM = 0x400,
    FONTFLAG_BITMAP = 0x800,
};

struct IntRect
{
    int x0;
    int y0;
    int x1;
    int y1;
};

struct Vertex_t
{
    Vertex_t() {}
    Vertex_t(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
    {
        m_Position = pos;
        m_TexCoord = coord;
    }
    void Init(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
    {
        m_Position = pos;
        m_TexCoord = coord;
    }

    Vector2D m_Position;
    Vector2D m_TexCoord;
};

//-----------------------------------------------------------------------------
// Purpose: Wraps contextless windows system functions
//-----------------------------------------------------------------------------
class ISurface : public IAppSystem
{
public:
    void SetRenderColor(int r, int g, int b, int a = 255) 
    {
        using Fn = void(__thiscall*)(ISurface*, int, int, int, int);
        return (*(Fn**)this)[15](this, r, g, b, a);
    }

    void PlaySound(const char* soundPath) 
    {
        using Fn = void(__thiscall*)(ISurface*, const char*);
        return (*(Fn**)this)[82](this, soundPath);
    }

    void SetTextColor(int r, int g, int b, int a = 255) 
    {
        using Fn = void(__thiscall*)(ISurface*, int, int, int, int);
        return (*(Fn**)this)[25](this, r, g, b, a);
    }

    void RenderPolygon(int n, Vertex_t* vertice, bool clipVertices = true) 
    {
        using Fn = void(__thiscall*)(ISurface*, int, Vertex_t*, bool);
        return (*(Fn**)this)[106](this, n, vertice, clipVertices);
    }

    void RenderFilledRect(int x, int y, int w, int h) 
    {
        using Fn = void(__thiscall*)(ISurface*, int, int, int, int);
        return (*(Fn**)this)[16](this, x, y, x + w, y + h);
    }

    void SetTexture(int id) 
    {
        using Fn = void(__thiscall*)(ISurface*, int);
        return (*(Fn**)this)[38](this, id);
    }

    inline void RenderTexturedRect(int x, int y, int w, int h) 
    {
        typedef void(__thiscall* Fn)(void*, int, int, int, int);
        return (*(Fn**)this)[41](this, x, y, w, h);
    }

    void SetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall) 
    {
        using Fn = void(__thiscall*)(ISurface*, int, const unsigned char*, int, int, int, bool);
        return (*(Fn**)this)[37](this, id, rgba, wide, tall, 0, false);
    }

    int CreateNewTextureID(bool procedural = false) 
    {
        using Fn = int(__thiscall*)(ISurface*, bool);
        return (*(Fn**)this)[43](this, procedural);
    }

    void RenderOutlinedRect(int x, int y, int w, int h) 
    {
        using Fn = void(__thiscall*)(ISurface*, int, int, int, int);
        return (*(Fn**)this)[18](this, x, y, x + w, y + h);
    }

    void RenderLine(int x1, int y1, int x2, int y2) {
        using Fn = void(__thiscall*)(ISurface*, int, int, int, int);
        return (*(Fn**)this)[19](this, x1, y1, x2, y2);
    }

    void SetTextFont(unsigned long font) 
    {
        using Fn = void(__thiscall*)(ISurface*, unsigned long);
        return (*(Fn**)this)[23](this, font);
    }

    void SetTextPos(int x, int y) 
    {
        using Fn = void(__thiscall*)(ISurface*, int, int);
        return (*(Fn**)this)[26](this, x, y);
    }

    void RenderText(const wchar_t* text, int textLen) {
        using Fn = void(__thiscall*)(ISurface*, const wchar_t*, int, int);
        return (*(Fn**)this)[28](this, text, textLen, 0);
    }

    unsigned long FontCreate() 
    {
        using Fn = unsigned int(__thiscall*)(ISurface*);
        return (*(Fn**)this)[71](this);
    }

    void SetFontGlyph(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanLines, int flags) 
    {
        using Fn = void(__thiscall*)(ISurface*, unsigned long, const char*, int, int, int, int, int, int, int);
        return (*(Fn**)this)[72](this, font, windowsFontName, tall, weight, blur, scanLines, flags, 0, 0);
    }

    void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall) {
        using Fn = void(__thiscall*)(ISurface*, unsigned long, const wchar_t*, int&, int&);
        return (*(Fn**)this)[79](this, font, text, wide, tall);
    }

    void UnlockCursor() 
    {
        using Fn = void(__thiscall*)(ISurface*);
        return (*(Fn**)this)[66](this);
    }

    RECT GetTextSizeRect(unsigned long font, const char* text)
    {
        size_t originalSize = strlen(text) + 1;
        const size_t newSize = 100;
        size_t convertedChars = 0;
        wchar_t wcstring[newSize];
        mbstowcs_s(&convertedChars, wcstring, originalSize, text, _TRUNCATE);

        RECT rect; int x, y;
        GetTextSize(font, wcstring, x, y);
        rect.left = x; rect.bottom = y;
        rect.right = x;
        return rect;
    }

    void DrawT(int X, int Y, Color Color, int Font, bool Center, const char* _Input, ...)
    {
        int apple = 0;
        char Buffer[2048] = { '\0' };
        va_list Args;

        va_start(Args, _Input);
        vsprintf_s(Buffer, _Input, Args);
        va_end(Args);

        size_t size = strlen(Buffer) + 1;
        wchar_t* WideBuffer = new wchar_t[size];
        mbstowcs_s(0, WideBuffer, size, Buffer, size - 1);
        int Width = 0, Height = 0;

        if (Center)
        {
            GetTextSize(Font, WideBuffer, Width, Height);
        }

        SetTextColor(Color.r(), Color.g(), Color.b(), Color.a());
        SetTextFont(Font);
        SetTextPos(X - (Width / 2), Y);
        RenderText(WideBuffer, wcslen(WideBuffer));

        return;
    }
};