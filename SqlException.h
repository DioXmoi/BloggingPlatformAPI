#pragma once

#include <string>
#include <stdexcept>

class SqlException : public std::runtime_error {
public:
    SqlException(const std::string& message, const std::string& detail = "") 
        : std::runtime_error{ message }
        , m_detail{ detail }
    {
    }

    const std::string& GetDetail() const { return m_detail; }

private:
    std::string m_detail;
};

class SqlEmptyAnswer : public SqlException {
public:
    SqlEmptyAnswer(const std::string& message, const std::string& detail = "")
        : SqlException{ message, detail }
    {
    }
};