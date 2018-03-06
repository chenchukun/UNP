#include <mysql/mysql.h>
#include <cstring>
#include <iostream>
using namespace std;

int main()
{
	MYSQL my_connection;
	mysql_init(&my_connection);
	if(!mysql_real_connect(&my_connection, "localhost", "root", "123456", "chat", 0, NULL, 0))
	{
		cout << "connect error" << endl;
	}
	char *ins = "insert into users values(291235, 'cck', '123456', 'img/head.png')";
	mysql_real_query(&my_connection, ins, strlen(ins));

	char *sel = "select * from users";
	mysql_real_query(&my_connection, sel, strlen(sel));

	MYSQL_RES *g_res = mysql_store_result(&my_connection);
	int iNum_rows = mysql_num_rows(g_res);
	int iNum_fields = mysql_num_fields(g_res);
	cout << iNum_rows << "    " << iNum_fields << endl;


	MYSQL_ROW g_row;
	while(g_row = mysql_fetch_row(g_res))
	{
		cout << g_row[0] << "    " << g_row[1] << "    " << g_row[2] << endl;
	}

	mysql_free_result(g_res);
	mysql_close(&my_connection);

	return 0;
}
