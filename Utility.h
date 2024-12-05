#pragma once

#include "Post.h"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <sstream>


namespace Utility {
	inline std::string ToString(const DateTime& dateTime) {
		std::stringstream stream{ };

		stream << dateTime;

		return stream.str();
	}


	inline std::string ToString(Post::Category category) {
		constexpr auto names = std::array{ "None",
			"Inspiration",
			"Reflections",
			"Travel",
			"Creativity",
			"Food",
			"News",
			"Education",
			"Technology",
			"Health",
			"Career",
			"Humor",
			"Movies",
			"TV series",
			"Music",
			"Games",
			"Animals",
			"Beauty",
			"Home decor",
			"Books",
			"Sports",
			"Art" };

		if (category >= Post::Category::MAX_CATEGORY || category < Post::Category::None) {
			return names[0];
		}

		return names[category];
	}

	inline Post::Category FromString(std::string_view str) {
		constexpr auto names = std::array{ "None",
		"Inspiration",
		"Reflections",
		"Travel",
		"Creativity",
		"Food",
		"News",
		"Education",
		"Technology",
		"Health",
		"Career",
		"Humor",
		"Movies",
		"TV series",
		"Music",
		"Games",
		"Animals",
		"Beauty",
		"Home decor",
		"Books",
		"Sports",
		"Art" };

		auto it{ std::ranges::find(names, str) };
		if (it == names.end()) {
			return Post::Category::None;
		}

		return static_cast<Post::Category>(std::distance(names.begin(), it));
	}


}