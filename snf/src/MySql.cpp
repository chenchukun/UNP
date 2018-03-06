#include "MySql.h"

namespace snf
{

MySql::~MySql()
{
	mysql_close(&myConnection);
}

int MySql::connect(const std::string &user, const std::string &password, 
		const std::string &database, const std::string &addr, int port)
{
	mysql_init(&myConnection);
	MYSQL *ret = mysql_real_connect(&myConnection, addr.c_str(), user.c_str(), 
		password.c_str(), database.c_str(), port, NULL, 0);
	if (ret == NULL) {
		return -1;
	}
	return 0;
}

int MySql::execute(const std::string &sql)
{
	return mysql_real_query(&myConnection, sql.c_str(), sql.size());
}

int MySql::action()
{
	return 0;
}

int MySql::commit()
{
	return 0;
}

std::vector<std::vector<Any>> MySql::fetchall()
{
	return std::vector<std::vector<Any>>();
}

int MySql::close() 
{
	return 0;
}

}
