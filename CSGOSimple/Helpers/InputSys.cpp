#include "InputSys.hpp"

#include "../SDK/SDK.hpp"
#include "../Menu/Menu.hpp"
#include "Configs.hpp"

InputSys::InputSys()
	: m_hTargetWindow(nullptr), m_ulOldWndProc(0)
{
}

InputSys::~InputSys()
{
	if (m_ulOldWndProc)
		SetWindowLongPtr(m_hTargetWindow, GWLP_WNDPROC, m_ulOldWndProc);
	m_ulOldWndProc = 0;
}

void InputSys::Initialize()
{
	D3DDEVICE_CREATION_PARAMETERS params;

	if (FAILED(g_D3DDevice9->GetCreationParameters(&params)))
		throw std::runtime_error("[InputSys] GetCreationParameters failed.");

	m_hTargetWindow = params.hFocusWindow;
	m_ulOldWndProc = SetWindowLongPtr(m_hTargetWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

	if (!m_ulOldWndProc)
		throw std::runtime_error("[InputSys] SetWindowLongPtr failed.");
}

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall InputSys::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Get().ProcessMessage(msg, wParam, lParam);

	if (Get().WasKeyPressed(g_Configs.misc.menuKey)) 
	{
		Menu.Opened = !Menu.Opened;
	}

	if (Menu.Opened) 
	{
		g_InputSystem->EnableInput(false);

	}
	else if (!Menu.Opened) 
	{
		g_InputSystem->EnableInput(true);
	}

	if (Menu.Opened && ImGui_ImplDX9_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	return CallWindowProcA((WNDPROC)Get().m_ulOldWndProc, hwnd, msg, wParam, lParam);
}

bool InputSys::ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_XBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_XBUTTONUP:
		return ProcessMouseMessage(msg, wParam, lParam);
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		return ProcessKeybdMessage(msg, wParam, lParam);
	default:
		return false;
	}
}

bool InputSys::ProcessMouseMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto key = VK_LBUTTON;
	auto state = KeyState::NONE;

	switch (msg) 
	{
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		state = msg == WM_MBUTTONUP ? KeyState::UP : KeyState::DOWN;
		key = VK_MBUTTON;
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		state = msg == WM_RBUTTONUP ? KeyState::UP : KeyState::DOWN;
		key = VK_RBUTTON;
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		state = msg == WM_LBUTTONUP ? KeyState::UP : KeyState::DOWN;
		key = VK_LBUTTON;
		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		state = msg == WM_XBUTTONUP ? KeyState::UP : KeyState::DOWN;
		key = (HIWORD(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
		break;
	default:
		return false;
	}

	if (state == KeyState::UP && m_iKeyMap[key] == KeyState::DOWN)
		m_iKeyMap[key] = KeyState::PRESSED;
	else
		m_iKeyMap[key] = state;

	return true;
}

bool InputSys::ProcessKeybdMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto key = wParam;
	auto state = KeyState::NONE;

	switch (msg) 
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		state = KeyState::DOWN;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		state = KeyState::UP;
		break;
	default:
		return false;
	}

	if (state == KeyState::UP && m_iKeyMap[int(key)] == KeyState::DOWN)
	{
		m_iKeyMap[int(key)] = KeyState::PRESSED;

		auto& hotkeyCallback = m_Hotkeys[key];

		if (hotkeyCallback)
			hotkeyCallback();
	}
	else 
	{
		m_iKeyMap[int(key)] = state;
	}

	return true;
}

KeyState InputSys::GetKeyState(std::uint32_t vk)
{
	return m_iKeyMap[vk];
}

bool InputSys::IsKeyDown(std::uint32_t vk)
{
	return m_iKeyMap[vk] == KeyState::DOWN;
}

bool InputSys::WasKeyPressed(std::uint32_t vk)
{
	if (m_iKeyMap[vk] == KeyState::PRESSED) 
	{
		m_iKeyMap[vk] = KeyState::UP;
		return true;
	}
	return false;
}

void InputSys::RegisterHotkey(std::uint32_t vk, std::function<void(void)> f)
{
	m_Hotkeys[vk] = f;
}

void InputSys::RemoveHotkey(std::uint32_t vk)
{
	m_Hotkeys[vk] = nullptr;
}