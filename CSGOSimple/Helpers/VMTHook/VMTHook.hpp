#pragma once

#include <cstring>
#include <cassert>

#include "Platform.hpp"

using proctT = void(*)();

class VMTBaseHook
{
protected:
	constexpr VMTBaseHook() = default;

public:
	~VMTBaseHook()
	{
		if (m_NewVMT)
			delete[](m_NewVMT - 1);
	}

	VMTBaseHook(const VMTBaseHook&) = delete;
	VMTBaseHook(VMTBaseHook&&) = delete;

	VMTBaseHook& operator=(const VMTBaseHook&) = delete;
	VMTBaseHook& operator=(VMTBaseHook&&) = delete;

protected:
	auto Initialize(proctT* oiriginalTable) -> void
	{
		m_OldVMT = oiriginalTable;

		size_t size = 0;
		while (m_OldVMT[size] && Platform::IsCodePtr(m_OldVMT[size]))
			++size;

		m_NewVMT = (new proctT[size + 1]) + 1;
		memcpy(m_NewVMT - 1, m_OldVMT - 1, sizeof(void*) * (size + 1));
	}

	constexpr auto LeakTable() -> void
	{
		m_NewVMT = nullptr;
	}

	auto HookInstance(void* inst) const -> void
	{
		auto& vtbl = *reinterpret_cast<proctT**>(inst);
		assert(vtbl == m_OldVMT || vtbl == m_NewVMT);
		vtbl = m_NewVMT;
	}

	auto UnhookInstance(void* inst) const -> void
	{
		auto& vtbl = *reinterpret_cast<proctT**>(inst);
		assert(vtbl == m_OldVMT || vtbl == m_NewVMT);
		vtbl = m_OldVMT;
	}

	auto InitializeAndHookInstance(void* inst) -> bool
	{
		auto& vtbl = *reinterpret_cast<proctT**>(inst);
		auto Initialized = false;
		if (!m_OldVMT)
		{
			Initialized = true;
			Initialize(vtbl);
		}
		HookInstance(inst);
		return Initialized;
	}

	template <typename Fn>
	auto HookFunction(Fn hookedFn, const std::size_t index) -> Fn
	{
		m_NewVMT[index] = (proctT)(hookedFn);
		return (Fn)(m_OldVMT[index]);
	}

	template<typename T>
	auto ApplyHook(std::size_t idx) -> void
	{
		T::m_Original = HookFunction(&T::Hook, idx);
	}

	template <typename Fn = uintptr_t>
	auto GetOriginalFunction(const int index) -> Fn
	{
		return (Fn)(m_OldVMT[index]);
	}

private:
	proctT* m_NewVMT = nullptr;
	proctT* m_OldVMT = nullptr;
};

class VMTSmartHook : VMTBaseHook
{
public:
	VMTSmartHook(void* classBase)
		: m_Class{ classBase }
	{
		InitializeAndHookInstance(classBase);
	}

	~VMTSmartHook()
	{
		UnhookInstance(m_Class);
	}

	VMTSmartHook(const VMTSmartHook&) = delete;
	VMTSmartHook(VMTSmartHook&&) = delete;

	VMTSmartHook& operator=(const VMTSmartHook&) = delete;
	VMTSmartHook& operator=(VMTSmartHook&&) = delete;

	auto Rehook() const -> void
	{
		HookInstance(m_Class);
	}

	auto Unhook() const -> void
	{
		UnhookInstance(m_Class);
	}

	using VMTBaseHook::ApplyHook;
	using VMTBaseHook::GetOriginalFunction;
	using VMTBaseHook::HookFunction;

private:
	void* m_Class = nullptr;
};

class VMTMultiHook : VMTBaseHook
{
public:
	constexpr VMTMultiHook() = default;

	~VMTMultiHook()
	{
		LeakTable();
	}

	VMTMultiHook(const VMTMultiHook&) = delete;
	VMTMultiHook(VMTMultiHook&&) = delete;

	VMTMultiHook& operator=(const VMTMultiHook&) = delete;
	VMTMultiHook& operator=(VMTMultiHook&&) = delete;

	using VMTBaseHook::ApplyHook;
	using VMTBaseHook::GetOriginalFunction;
	using VMTBaseHook::HookFunction;
	using VMTBaseHook::HookInstance;
	using VMTBaseHook::UnhookInstance;
	using VMTBaseHook::Initialize;
	using VMTBaseHook::InitializeAndHookInstance;
};