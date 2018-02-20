#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include <ctime>
#include <sys/time.h>
#include <cstdint>
#include <assert.h>
#include "common.h"

NAMESPACE_START

class Timestamp
{
	friend Timestamp operator+(const Timestamp &left, const Timestamp &right);

	friend Timestamp operator-(const Timestamp &left, const Timestamp &right);

	friend bool operator>(const Timestamp &left, const Timestamp &right);

	friend bool operator>=(const Timestamp &left, const Timestamp &right);

	friend bool operator<(const Timestamp &left, const Timestamp &right);

	friend bool operator<=(const Timestamp &left, const Timestamp &right);

	friend bool operator==(const Timestamp &left, const Timestamp &right);

	friend bool operator!=(const Timestamp &left, const Timestamp &right);

public:
	enum {TIME_MSEC=1000, TIME_USEC=1000000};

	Timestamp();

	Timestamp(uint64_t usec);

	Timestamp(const Timestamp &timestamp);

	~Timestamp(){}

	Timestamp& operator=(const Timestamp &timestamp);

	uint64_t sec() const;

	uint64_t usec() const;

	uint64_t msec() const;

	inline static Timestamp now();

	Timestamp& operator+=(const Timestamp &timestamp);

	Timestamp& operator-=(const Timestamp &timestamp);

private:

	uint64_t usec_;
};


inline Timestamp::Timestamp()
{
	struct timeval tv;
	int ret = gettimeofday(&tv, NULL);
	assert(ret != -1);
	this->usec_ = tv.tv_sec*TIME_USEC + tv.tv_usec;
}


inline Timestamp::Timestamp(uint64_t usec)
{
	this->usec_ = usec;
}


inline Timestamp::Timestamp(const Timestamp &timestamp)
	:usec_(timestamp.usec_)
{
}


inline Timestamp& Timestamp::operator=(const Timestamp &timestamp)
{
	this->usec_ = timestamp.usec_;
	return *this;
}


inline uint64_t Timestamp::sec() const
{
	return static_cast<uint64_t>(this->usec_/TIME_USEC);
}


inline uint64_t Timestamp::usec() const
{
	return this->usec_;
}


inline uint64_t Timestamp::msec() const
{
	return static_cast<uint64_t>(this->usec_/TIME_MSEC);
}


inline Timestamp Timestamp::now()
{
	return Timestamp();
}


inline Timestamp& Timestamp::operator+=(const Timestamp &timestamp)
{
	this->usec_ += timestamp.usec_;
	return *this;
}


inline Timestamp& Timestamp::operator-=(const Timestamp &timestamp)
{
	if (this->usec_ < timestamp.usec_) {
		this->usec_ = 0;
	}
	else {
		this->usec_ -= timestamp.usec_;
	}
	return *this;
}


inline Timestamp operator+(const Timestamp &left, const Timestamp &right)
{
	Timestamp ts = Timestamp(left);
	ts.usec_ += right.usec_;
	return ts;
}


inline Timestamp operator-(const Timestamp &left, const Timestamp &right)
{
	if (left.usec_ <= right.usec_) {
		return Timestamp(0);
	}
	Timestamp ts = Timestamp(left);
	ts.usec_ -= right.usec_;
	return ts;
}

inline bool operator>(const Timestamp &left, const Timestamp &right)
{
	return left.usec_ > right.usec_;
}


inline bool operator>=(const Timestamp &left, const Timestamp &right)
{
	return left.usec_ >= right.usec_;
}


inline bool operator<(const Timestamp &left, const Timestamp &right)
{
	return left.usec_ < right.usec_;
}


inline bool operator<=(const Timestamp &left, const Timestamp &right)
{
	return left.usec_ <= right.usec_;
}


inline bool operator==(const Timestamp &left, const Timestamp &right)
{
	return left.usec_ == right.usec_;
}

inline bool operator!=(const Timestamp &left, const Timestamp &right)
{
	return !(left == right);
}

NAMESPACE_END

#endif
