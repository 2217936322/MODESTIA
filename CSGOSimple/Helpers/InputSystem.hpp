#pragma once

#define NOMINMAX

#include <Windows.h>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <d3d9.h>

#include "Singleton.hpp"

#include "../SDK/Misc/Enums.hpp"

DEFINE_ENUM_FLAG_OPERATORS(KeyState);

class InputSystem : public Singleton<InputSystem>
{
    friend class Singleton<InputSystem>;

    InputSystem();
    ~InputSystem();

public:
    void Initialize();

    HWND GetMainWindow() const { return m_hTargetWindow; }

    KeyState GetKeyState(uint32_t vk);
    bool IsKeyDown(uint32_t vk);
    bool WasKeyPressed(uint32_t vk);

    void RegisterHotkey(uint32_t vk, std::function<void(void)> f);
    void RemoveHotkey(uint32_t vk);

    HWND m_hTargetWindow;

private:
    static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    bool ProcessMouseMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    bool ProcessKeybdMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    LONG_PTR m_ulOldWndProc;
    KeyState m_iKeyMap[256];

    std::function<void(void)> m_Hotkeys[256];
};