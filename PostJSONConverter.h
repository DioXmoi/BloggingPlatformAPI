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

		j["id"] = post.GetID();
		j["title"] = post.GetTitle();
		j["content"] = post.GetContent();
		j["category"] = Utility::ToString(post.GetCategory());
		j["tags"] = post.GetTags().GetData();
		j["createdAt"] = Utility::ToString(post.GetCreatedAt());
		j["updatedAt"] = Utility::ToString(post.GetUpdateAt());

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


	inline Post ParseForHttpPost(std::string_view str) {
		json j{ json::parse(str) };

		Post post{ };

		post.SetTitle(j.at("title").get<std::string>());
		post.SetContent(j.at("content").get<std::string>());
		post.SetCategory(Utility::FromString(j.at("category").get<std::string>()));
		post.SetTags(Utility::CreateTags(j.at("tags").get<std::vector<std::string>>()));

		return post;
	}
}