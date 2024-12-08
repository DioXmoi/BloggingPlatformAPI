#pragma once


#include <string>
#include <memory>
#include <libpq-fe.h>

#include "SqlException.h"



class Connection {
public:
	Connection()
		: m_host{ "localhost" }
		, m_port{ 5432 }
		, m_db_name{ "BloggingPlatformAPI" }
		, m_user{ "postgres" }
		, m_pass{ "12noreply" }
	{
		EstablishConnection();
	}

	std::shared_ptr<PGconn> GetConnection() const {
		return m_connection;
	}

private:
	void EstablishConnection() {
		m_connection.reset(
			PQsetdbLogin(m_host.c_str(), 
				std::to_string(m_port).c_str(), 
				nullptr, 
				nullptr, 
				m_db_name.c_str(), 
				m_user.c_str(), 
				m_pass.c_str()), 
			&PQfinish);

		if (PQstatus(m_connection.get()) != CONNECTION_OK && PQsetnonblocking(m_connection.get(), 1) != 0) {
			throw SqlException("PostgreSQL connection failed", PQerrorMessage(m_connection.get()));
		}
	}

private:
	std::string m_host{ };
	int m_port{ };
	std::string m_db_name{ };
	std::string m_user{ };
	std::string m_pass{ };
	std::shared_ptr<PGconn>  m_connection{ };
};