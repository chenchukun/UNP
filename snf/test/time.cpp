#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
#include <iostream>
using namespace std;

// 获取时间戳，从1970年1月1日0点到到现在的秒数(0时区)
void testTime()
{
	cout << "--------testTime---------" << endl;
	time_t now = time(NULL);
	cout << now << endl;
	cout << endl;
}


// 获取时间戳，精确到微妙
void testGettimeofday()
{
	cout << "--------testGettimeofday---------" << endl;
	struct timeval tmv;
	int ret = gettimeofday(&tmv, NULL);
	assert(ret != -1);
	cout << "tv_sec: " << tmv.tv_sec << endl;
	cout << "tv_usec: " << tmv.tv_usec << endl;
	cout << endl;
}


// 将时间戳转化为可打印格式
void testCtime()
{
	cout << "--------testCtime---------" << endl;
	time_t now = time(NULL);
	const char *ct = ctime(&now);
	assert(ct != NULL);
	cout << "ctime: " << ct << endl;
	char rct[26];
	auto ret = ctime_r(&now, rct);
	assert(ret != NULL);
	cout << "ctime_r: " << rct << endl;
	cout << endl;
}


// 将时间戳转化为分解时间，使用0时区
void testGmtime()
{
	cout << "--------testGmtime---------" << endl;
	time_t now = time(NULL);
	struct tm *tm = gmtime(&now);
	assert(tm != NULL);
	cout << "gmtime: " << endl
		 << "tm_sec: " << tm->tm_sec << "  "
		 << "tm_min: " << tm->tm_min << "  "
		 << "tm_hour: " << tm->tm_hour << "  "
		 << "tm_mday: " << tm->tm_mday << "  "
		 << "tm_mon: " << tm->tm_mon << "  "		// 0 - 11
		 << "tm_year: " << tm->tm_year << "  "		// year = 1900 + tm_year
		 << "tm_wday: " << tm->tm_wday << "  "		// 0为周日
		 << "tm_yday: " << tm->tm_yday << "  "
		 << "tm_isdst: " << tm->tm_isdst << "  "
		 << endl;

	struct tm tmr;
	tm = gmtime_r(&now, &tmr);
	assert(tm != NULL);
	cout << "gmtime_r: " << endl
		 << "tm_sec: " << tmr.tm_sec << "  "
		 << "tm_min: " << tmr.tm_min << "  "
		 << "tm_hour: " << tmr.tm_hour << "  "
		 << "tm_mday: " << tmr.tm_mday << "  "
		 << "tm_mon: " << tmr.tm_mon << "  "
		 << "tm_year: " << tmr.tm_year << "  "
		 << "tm_wday: " << tmr.tm_wday << "  "
		 << "tm_yday: " << tmr.tm_yday << "  "
		 << "tm_isdst: " << tmr.tm_isdst << "  "
		 << endl;
	cout << endl;
}


// 将时间戳转化为分解时间，使用当前所在时区
void testLocaltime()
{
	cout << "--------testLocaltime---------" << endl;
	time_t now = time(NULL);
	struct tm *tm = localtime(&now);
	assert(tm != NULL);
	cout << "localtime: " << endl
		 << "tm_sec: " << tm->tm_sec << "  "
		 << "tm_min: " << tm->tm_min << "  "
		 << "tm_hour: " << tm->tm_hour << "  "
		 << "tm_mday: " << tm->tm_mday << "  "
		 << "tm_mon: " << tm->tm_mon << "  "
		 << "tm_year: " << tm->tm_year << "  "
		 << "tm_wday: " << tm->tm_wday << "  "
		 << "tm_yday: " << tm->tm_yday << "  "
		 << "tm_isdst: " << tm->tm_isdst << "  "
		 << endl;

	struct tm tmr;
	tm = localtime_r(&now, &tmr);
	assert(tm != NULL);
	cout << "localtime_r: " << endl
		 << "tm_sec: " << tmr.tm_sec << "  "
		 << "tm_min: " << tmr.tm_min << "  "
		 << "tm_hour: " << tmr.tm_hour << "  "
		 << "tm_mday: " << tmr.tm_mday << "  "
		 << "tm_mon: " << tmr.tm_mon << "  "
		 << "tm_year: " << tmr.tm_year << "  "
		 << "tm_wday: " << tmr.tm_wday << "  "
		 << "tm_yday: " << tmr.tm_yday << "  "
		 << "tm_isdst: " << tmr.tm_isdst << "  "
		 << endl;
	cout << endl;
}


// 将本地时区的分解时间转换为时间戳
// mktime 会更改tm结构体
void testMktime()
{
	cout << "--------testMktime---------" << endl;
	time_t now = time(NULL);
	struct tm *tm = gmtime(&now);
	assert(tm != NULL);
	cout << "now: " << now << endl;

	time_t ret = mktime(tm);
	cout << "tm is gmtime: " << endl;
	cout << "ret: " << ret << endl;

	tm = localtime(&now);
	assert(tm != NULL);
//	tm->tm_hour -= 20;
	ret = mktime(tm);
	cout << "tm is localmtime: " << endl;
	cout << "ret: " << ret << endl;
	cout << endl;
}


// 将分解时间转换为可打印格式
void testAsctime()
{
	cout << "--------testAsctime---------" << endl;
	time_t now = time(NULL);
	struct tm *tm = gmtime(&now);
	assert(tm != NULL);

	const char *asct = asctime(tm);
	assert(asct != NULL);
	cout << "tm is gmtime: " << endl;
	cout << asct << endl;

	tm = localtime(&now);
	assert(tm != NULL);
	char asctr[26];
	asct = asctime_r(tm, asctr);
	assert(asct != NULL);
	cout << "tm is localtime asctime_r: " << endl;
	cout << asctr << endl;
	cout << endl;
}


// 将分解时间按照给定格式转换为可打印时间
// %b: 月份缩写				%c: 日期和时间Tue Feb 1 00:00:00 2017
// %d: 两位日				%D: 美国格式时间 %m/%d/%y 03/08/17
// %F: ISO日期2017-03-08	%H: 24小时制小时
// %I: 12小时制小时			%m:	十进制月份
// %M: 分					%R: 24小时制时间21:35
// %S: 秒					%T: 时间 21:35:00
// %u: 星期1-7				%w: 星期0-6
// %Y: 年2017
void testStrftime()
{
	cout << "--------testStrftime---------" << endl;
	time_t now = time(NULL);
	struct tm *tm = localtime(&now);
	assert(tm != NULL);

	char buff[100];
	// 返回buff的长度，不包含空字符
	size_t ret = strftime(buff, 100, "%Y-%m-%d %H:%M:%S %Z", tm);
	assert(ret != 0);
	cout << buff << endl;
	cout << endl;
}


// 将可打印格式时间转换为分解时间
// strptime不设置tm结构中那些未获format说明符初始化的字段
void testStrptime()
{
	cout << "--------testStrptime---------" << endl;
	struct tm ptm;
	memset(&ptm, 0, sizeof(ptm));
	char *nextChar = strptime("2017-03-08 20:06:18", "%Y-%m-%d %H:%M:%S", &ptm);
	assert(nextChar != NULL);	// 返回buff未被处理的第一个字符
	cout << "tm_sec: " << ptm.tm_sec << "  "
		 << "tm_min: " << ptm.tm_min << "  "
		 << "tm_hour: " << ptm.tm_hour << "  "
		 << "tm_mday: " << ptm.tm_mday << "  "
		 << "tm_mon: " << ptm.tm_mon << "  "
		 << "tm_year: " << ptm.tm_year << "  "
		 << "tm_wday: " << ptm.tm_wday << "  "
		 << "tm_yday: " << ptm.tm_yday << "  "
		 << "tm_isdst: " << ptm.tm_isdst << "  "
		 << endl;
	time_t ret = mktime(&ptm);
	cout << "time_t: " << ret << endl;
	cout << endl;
}


// 怎么用？
void testTimes()
{
	cout << "--------testTimes---------" << endl;
	struct tms tms;
	clock_t ret = times(&tms);
	sleep(1);
	assert(ret != -1);
	cout << "tms_utime: " << tms.tms_utime << "  "
		 << "tms_stime: " << tms.tms_stime << "  "
		 << "tms_cutime: " << tms.tms_cutime << "  "
		 << "tms_cstime: " << tms.tms_cstime << "  "
		 << endl;
}


int main()
{
	testTime();
	testGettimeofday();
	testCtime();
	testGmtime();
	testLocaltime();
	testMktime();
	testAsctime();
	testStrftime();
	testStrptime();
	testTimes();
	return 0;
}


