#pragma once
#include <cstdint>
#include <utility>
#include <array>

namespace Platform
{
	namespace Detailt
	{
		constexpr auto ProcessPatternByte(const std::pair<char, char> pair) -> std::pair<std::uint8_t, bool>
		{
			return { std::uint8_t(pair.first), pair.second == 'x' };
		}

		template <std::size_t N>
		constexpr auto SelectPatternByte(const char(&pattern)[N], const char(&mask)[N], std::size_t n) -> std::pair<char, char>
		{
			return { pattern[n], mask[n] };
		}

		template <typename>
		struct SigProcessor {};

		template <std::size_t... Indices>
		struct SigProcessor<std::index_sequence<Indices...>>
		{
			template <std::size_t N>
			static constexpr auto process(const char(&pattern)[N], const char(&mask)[N])->std::array<std::pair<std::uint8_t, bool>, N>
			{
				return { ProcessPatternByte(SelectPatternByte(pattern, mask, Indices))... };
			}
		};
	}

	auto GetInterface(const char* moduleName, const char* interfaceName) -> void*;
	auto GetModuleInfo(const char* moduleName)->std::pair<std::uintptr_t, std::size_t>;
	auto IsCodePtr(void* ptr) -> bool;
	auto GetExport(const char* moduleName, const char* exportName) -> void*;

	template <std::size_t N>
	auto FindPattern(const char* moduleName, const char(&pattern)[N], const char(&mask)[N]) -> std::uintptr_t
	{
		const std::array<std::pair<std::uint8_t, bool>, N> signature = detail::SigProcessor<std::make_index_sequence<N>>::process(pattern, mask);

		const auto info = GetModuleInfo(moduleName);
		const auto address = reinterpret_cast<std::uint8_t*>(info.first);
		const auto size = info.second;

		const auto ret = std::search(address, address + size, signature.begin(), signature.end(),
			[](std::uint8_t curr, std::pair<std::uint8_t, bool> currPattern)
			{
				return (!currPattern.second) || curr == currPattern.first;
			});

		return ret == address + size ? 0 : std::uintptr_t(ret);
	}
}