#ifndef MYSQL_H
#define MYSQL_H
#include "Sql.h"
#include <mysql/mysql.h>

namespace snf
{

class MySql : public Sql
{
public:
	virtual int connect(const std::string &user, const std::string &password, 
		const std::string &database="", const std::string &addr="localhost", int port=3306);

	virtual ~MySql();

	virtual int execute(const std::string &sql);

	virtual int action();

	virtual int commit();

	virtual std::vector<std::vector<Any>> fetchall();

	virtual int close();

private:
	MYSQL myConnection;
};

}

#endif
