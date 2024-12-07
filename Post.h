#pragma once


#include <algorithm>
#include <chrono>
#include <vector>
#include <string>
#include <string_view>


using ID = int;
using DateTime = std::chrono::system_clock::time_point;


class Post {
public:
	enum Category {
		None,
		Inspiration,
		Reflections,
		Travel,
		Creativity,
		Food,
		News,
		Education,
		Technology,
		Health,
		Career,
		Humor,
		Movies,
		TVSeries,
		Music,
		Games,
		Animals,
		Beauty,
		HomeDecor,
		Books,
		Sports,
		Art,
		MAX_CATEGORY
	};


	class Tags {
	public:
		bool Add(std::string_view tag) { 
			if (tag.empty()) { return false; }
			auto found{ std::ranges::find(m_data, tag) };
			if (found != m_data.end()) {
				return false;
			}

			m_data.push_back(tag.data());

			return true;
		}

		const std::vector<std::string>& GetData() const { return m_data; }

	private:
		std::vector<std::string> m_data;
	};


	Post() = default;

	Post(ID id,
		std::string_view title,
		std::string_view content,
		Category category = Category::None,
		Tags tags = Tags(),
		const DateTime& createdAt = std::chrono::system_clock::now(),
		const DateTime& updateAt = std::chrono::system_clock::now()
	)
		: m_id{ id }
		, m_title{ title }
		, m_content{ content }
		, m_category{ category }
		, m_tags{ tags }
		, m_createdAt{ createdAt }
		, m_updateAt{ updateAt }
	{
	}


	//gets
	ID GetID() const { return m_id; }
	std::string GetTitle() const { return m_title; }
	std::string GetContent() const { return m_content; }
	Category GetCategory() const { return m_category; }
	Tags GetTags() const { return m_tags; }
	DateTime GetCreatedAt() const { return m_createdAt; }
	DateTime GetUpdateAt() const { return m_updateAt; }

	//sets
	bool SetTitle(std::string_view title) {
		if (title.empty()) {
			return false;
		}

		UpdateDateTime();
		m_title = title;

		return true;
	}

	bool SetContent(std::string_view content) {
		if (content.empty()) {
			return false;
		}

		UpdateDateTime();
		m_content = content;

		return true;
	}

	bool SetCategory(Category category) {
		if (category >= MAX_CATEGORY || category < Category::None) {
			return false;
		}

		UpdateDateTime();
		m_category = category;

		return true;
	}

	bool SetTags(Tags tags) {
		UpdateDateTime();
		m_tags = tags;

		return true;
	}

private:

	void UpdateDateTime() { m_updateAt = std::chrono::system_clock::now(); }

private:

	ID m_id{ };
	std::string m_title{ };
	std::string m_content{ };
	Category m_category{ };
	Tags m_tags{ };
	DateTime m_createdAt{ };
	DateTime m_updateAt{ };

};