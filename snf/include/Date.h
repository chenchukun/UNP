#ifndef DATE_H
#define DATE_H

#include "Timestamp.h"
#include <string>
#include <iostream>
#include <cstring>

namespace snf
{

class Date
{
	friend bool operator==(const Date &left, const Date &right);
	friend bool operator!=(const Date &left, const Date &right);
	friend bool operator>(const Date &left, const Date &right);
	friend bool operator>=(const Date &left, const Date &right);
	friend bool operator<(const Date &left, const Date &right);
	friend bool operator<=(const Date &left, const Date &right);
public:
	enum {FORMAT_BUFF_SIZE=128};

	Date();

	Date(const Timestamp &timestamp);

	Date(const std::string &stime, const std::string &format);

	Date(const Date &date);

	~Date(){}

	Date& operator=(const Date &date);

	void setFormat(const std::string &format);

	std::string format() const;

	std::string format(const std::string &format) const;

	Timestamp getTimestamp() const;

	int update(const Timestamp &timestamp);

	int update(const std::string &stime, const std::string &format);

	int getYear() const;

	int getMonth() const;

	int getDay() const;

	int getHour() const;

	int getMinute() const;

	int getSecond() const;

private:

	struct tm mtm;

	std::string mformat;
};

inline Date::Date()
	:Date(Timestamp::now())
{
}

inline Date::Date(const Timestamp &timestamp)
	: mformat("%Y-%m-%d %H:%M:%S")
{
	assert(update(timestamp) == 0);
}


inline Date::Date(const std::string &stime, const std::string &format)
{
	memset(&mtm, 0, sizeof(mtm));
	char *next = strptime(stime.c_str(), format.c_str(), &mtm);
	assert(next != NULL);
	this->mformat = format;
}


Date::Date(const Date &date)
	:mtm(date.mtm), mformat(date.mformat)
{
}


Date& Date::operator=(const Date &date)
{
	this->mtm = date.mtm;
	this->mformat = date.mformat;
	return *this;
}


int Date::update(const Timestamp &timestamp)
{
	time_t t = timestamp.sec();
	if( localtime_r(&t, &mtm) == NULL ) {
		return -1;
	}	
	return 0;
}


inline void Date::setFormat(const std::string &format)
{
	this->mformat = format;
}


inline std::string Date::format() const
{
	char buff[FORMAT_BUFF_SIZE];
	size_t ret = strftime(buff, FORMAT_BUFF_SIZE, mformat.c_str(), &mtm);
	if (ret == 0) {
		return std::string("");
	}
	return std::string(buff);
}


inline std::string Date::format(const std::string &format) const
{
	char buff[FORMAT_BUFF_SIZE];
	size_t ret = strftime(buff, FORMAT_BUFF_SIZE, format.c_str(), &mtm);
	if (ret == 0) {
		return std::string("");
	}
	return std::string(buff);
}


inline Timestamp Date::getTimestamp() const
{
	struct tm tm = mtm;
	time_t st = mktime(&tm);
	if (st == -1) {
		return Timestamp(0);
	}
	return Timestamp(st*Timestamp::TIME_USEC);
}


inline int Date::getYear() const
{
	return mtm.tm_year + 1900;
}


inline int Date::getMonth() const
{
	return mtm.tm_mon + 1;
}


inline int Date::getDay() const
{
	return mtm.tm_mday;
}


inline int Date::getHour() const
{
	return mtm.tm_hour;
}


inline int Date::getMinute() const
{
	return mtm.tm_min;
}


inline int Date::getSecond() const
{
	return mtm.tm_sec;
}


bool operator==(const Date &left, const Date &right)
{
	// 注意: ^ 的优先级小于 + 
	int ret = (left.getYear() ^ right.getYear())
		+ (left.getMonth() ^ right.getMonth())
		+ (left.getDay() ^ right.getDay())
		+ (left.getHour() ^ right.getHour())
		+ (left.getMinute() ^ right.getMinute())
		+ (left.getSecond() ^ right.getSecond());
	return (ret==0);
}

bool operator!=(const Date &left, const Date &right)
{
	return !(left == right);
}


bool operator>(const Date &left, const Date &right)
{
	if (left.getYear() <= right.getYear()) {
		return false;
	}
	if (left.getMonth() <= right.getMonth()) {
		return false;
	}
	if (left.getDay() <= right.getDay()) {
		return false;
	}
	if (left.getHour() <= right.getHour()) {
		return false;
	}
	if (left.getMinute() <= right.getMinute()) {
		return false;
	}
	if (left.getSecond() <= right.getSecond()) {
		return false;
	}
	return true;
}


bool operator>=(const Date &left, const Date &right)
{
	return (left>right || left==right);
}


bool operator<(const Date &left, const Date &right)
{
	return !(left >= right);
}


bool operator<=(const Date &left, const Date &right)
{
	return !(left > right);
}

}

#endif
