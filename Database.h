#pragma once


#include <string>
#include <iostream>
#include <libpq-fe.h>

#include "Connection.h"
#include "SqlException.h"
#include "Post.h"
#include "Utility.h"


class Database {
public:

	Database() = default;

	std::string Insert(const Post& post) {
		std::string query{ "INSERT INTO posts (Title, Content, Category, Tags) VALUES ($1, $2, $3, $4) RETURNING to_json(posts.*) AS json_data" };

		std::string title = post.GetTitle();
		std::string content = post.GetContent();
		std::string category = Utility::ToString(post.GetCategory());
		std::string tags = Utility::FormatArray(post.GetTags().GetData());
		const char* params[] = {
			title.c_str(),
			content.c_str(),
			category.c_str(),
			tags.c_str()
		};

		int lengths[] = {
			static_cast<int>(title.length()),
			static_cast<int>(content.length()),
			static_cast<int>(category.length()),
			static_cast<int>(tags.length())
		};

		const int formats[] = { 0, 0, 0, 0 };
		PGresult* res{ PQexecParams(
			m_connection.GetConnection().get(), 
			query.c_str(), 
			4, 
			NULL, 
			params, 
			lengths,
			formats, 
			0) };


		if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			PQclear(res);
			throw SqlException("PostgreSQL query failed", PQerrorMessage(m_connection.GetConnection().get()));
		}

		std::string json{ PQgetvalue(res, 0, 0) };

		PQclear(res);

		return json;
	}

private:
	Connection m_connection{ };
};