#pragma once


#include <string>
#include <iostream>
#include <vector>
#include <libpq-fe.h>

#include "Connection.h"
#include "SqlException.h"
#include "Post.h"
#include "Utility.h"


class Database {
public:
	Database() = default;

	//return json post
	std::string Insert(const Post& post) const {
		SqlQuerySender query{
			"INSERT INTO posts (Title, Content, Category, Tags) VALUES ($1, $2, $3, $4) RETURNING to_json(posts.*) AS json_data",
			SqlQueryData{ std::vector<std::string> {
			post.GetTitle(),
			post.GetContent(),
			Utility::ToString(post.GetCategory()),
			Utility::FormatArray(post.GetTags().GetData()) } }
		};

		const int formats[] = { 0, 0, 0, 0 };
		return query.Send(m_connection, formats);
	}

	//return json post
	std::string Update(const Post& post) const {
		SqlQuerySender query{
			"UPDATE posts SET Title = $1, Content = $2, Category = $3, Tags = $4, updatedat = now() WHERE Id = $5 RETURNING to_json(posts.*) AS json_data",
			SqlQueryData{ std::vector<std::string> {
			post.GetTitle(),
			post.GetContent(),
			Utility::ToString(post.GetCategory()),
			Utility::FormatArray(post.GetTags().GetData()),
			std::to_string(post.GetID()) } }
		};

		const int formats[] = { 0, 0, 0, 0, 0 };
		return query.Send(m_connection, formats);
	}

	//return json post
	std::string Delete(int id) const {
		SqlQuerySender query{
			"DELETE FROM posts WHERE Id = $1 RETURNING to_json(posts.*) AS json_data",
			SqlQueryData{ std::vector<std::string> { std::to_string(id) } }
		};

		const int formats[] = { 0 };
		return query.Send(m_connection, formats);
	}

	//return json post
	std::string Select(int id) const {
		SqlQuerySender query{
			"SELECT to_json(posts.*) FROM posts WHERE Id = $1",
			SqlQueryData{ std::vector<std::string> { std::to_string(id) } }
		};

		const int formats[] = { 0 };
		return query.Send(m_connection, formats);
	}

	//return array json posts
	std::string Select() const {
		SqlQuerySender query{
			"SELECT json_agg(to_json(posts.*)) FROM posts",
			SqlQueryData{{ }}
		};

		return query.Send(m_connection, NULL);
	}

	//return json post
	std::string Select(const std::string& term) const {
		SqlQuerySender query{
			"SELECT json_agg(to_json(posts.*)) FROM posts WHERE title ILIKE $1 OR content ILIKE $1 OR category::TEXT ILIKE $1",
			SqlQueryData{ std::vector<std::string> { "%" + term + "%" } }
		};

		const int formats[] = { 0 };
		return query.Send(m_connection, formats);
	}

private:

	class SqlQueryData {
	public:
		SqlQueryData(std::vector<std::string>&& data) noexcept
			: m_data{ data }
		{
		}

		SqlQueryData(SqlQueryData&& data) noexcept
			: m_data{ std::move(data.m_data) }
		{
		}

		int Size() const { return static_cast<int>(m_data.size()); }

		std::vector<const char*> GetParameters() const {
			std::vector<const char*> res;
			for (const auto& item : m_data) {
				res.emplace_back(item.c_str());
			}

			return res;
		}

		std::vector<int> GetLengths() const {
			std::vector<int> res;
			for (const auto& item : m_data) {
				res.emplace_back(static_cast<int>(item.length()));
			}

			return res;
		}

	private:
		std::vector<std::string> m_data;
	};


	class SqlQuerySender {
	public:
		SqlQuerySender(std::string&& query, SqlQueryData&& data) noexcept
			: m_query{ query }
			, m_data{ std::move(data) }
		{
		}

		std::string Send(const Connection& conn, const int formats[]) const {
			PGresult* res{ PQexecParams(
				conn.GetConnection().get(),
				m_query.c_str(),
				m_data.Size(),
				NULL,
				m_data.GetParameters().data(),
				m_data.GetLengths().data(),
				formats,
				0) };


			if (PQresultStatus(res) != PGRES_TUPLES_OK) {
				PQclear(res);
				throw SqlException("PostgreSQL query failed", PQerrorMessage(conn.GetConnection().get()));
			}
			else if (PQntuples(res) == 0) {
				PQclear(res);
				throw SqlEmptyAnswer("PostgreSQL query returned 0 rows", m_query);
			}

			std::string json{ PQgetvalue(res, 0, 0) };

			PQclear(res);

			return json;
		}

	private:
		std::string m_query;
		SqlQueryData m_data;
	};

private:
	Connection m_connection{ };
};