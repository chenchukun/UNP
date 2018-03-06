#ifndef SQL_H
#define SQL_H
#include <string>
#include <vector>
#include "Any.h"

namespace snf
{

class Sql
{
public:
	virtual int connect(const std::string &user, const std::string &password, 
			const std::string &database="", const std::string &addr="localhost", int port=0) = 0;

	virtual ~Sql() {}

	virtual int execute(const std::string &sql) = 0;

	virtual int action() = 0;

	virtual int commit() = 0;

	virtual std::vector<std::vector<Any>> fetchall() = 0;

	virtual int close() = 0;

};

}

#endif
