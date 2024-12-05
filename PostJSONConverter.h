#pragma once

#include "Post.h"
#include "Utility.h"

#include <iostream>

#include <string>
#include <string_view>
#include <algorithm>
#include <vector>

#include <nlohmann/json.hpp>


using json = nlohmann::json;

namespace PostJSONConverter {
	inline std::string Serialize(const Post& post) {
		json j;

		j["ID"] = post.GetID();
		j["Title"] = post.GetTitle();
		j["Content"] = post.GetContent();
		j["Category"] = Utility::ToString(post.GetCategory());
		j["Tags"] = post.GetTags();
		j["CreatedAt"] = Utility::ToString(post.GetCreatedAt());
		j["UpdatedAt"] = Utility::ToString(post.GetUpdateAt());

		return j.dump(4);
	}

	inline json Serialize(const std::vector<Post>& posts) {
		if (posts.empty()) {
			return json::array();
		}

		json j{ json::array() };
		std::ranges::transform(posts, std::back_inserter(j),
			[](const Post& post) { return Serialize(post); });

		return j;
	}

	//From json without CreatedAt and UpdatedAt
	inline Post Parse(std::string_view str) {
		json j{ json::parse(str) };

		ID id{ j.at("ID").get<ID>() };
		std::string title{ j.at("Title").get<std::string>() };
		std::string content{ j.at("Content").get<std::string>() };
		Post::Category category{ Utility::FromString(j.at("Category").get<std::string>()) };
		std::string tags{ j.at("Tags").get<std::string>() };

		return Post{ id, title, content, category, tags };
	}
}