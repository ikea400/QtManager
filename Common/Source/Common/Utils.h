#pragma once
#include <unordered_map>
#include <string>

namespace  Ikea400::Utils
{
	// credtits: https://www.cppstories.com/2021/heterogeneous-access-cpp20/
	struct string_hash {
		using is_transparent = void;
		[[nodiscard]] size_t operator()(const char* txt) const {
			return std::hash<std::string_view>{}(txt);
		}
		[[nodiscard]] size_t operator()(std::string_view txt) const {
			return std::hash<std::string_view>{}(txt);
		}
		[[nodiscard]] size_t operator()(const std::string& txt) const {
			return std::hash<std::string>{}(txt);
		}
	};

	template <typename T>
	using unordored_string_map = std::unordered_map<std::string, T, string_hash, std::equal_to<>>;

	int DoSomethingUseful(int value);
}