#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include <ctime>
#include <sys/time.h>
#include <cstdint>
#include <assert.h>

namespace snf
{

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

	Timestamp(uint64_t aUsec);

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

	uint64_t mUsec;
};


inline Timestamp::Timestamp()
{
	struct timeval tv;
	int ret = gettimeofday(&tv, NULL);
	assert(ret != -1);
	this->mUsec = tv.tv_sec*TIME_USEC + tv.tv_usec;
}


inline Timestamp::Timestamp(uint64_t aUsec)
{
	this->mUsec = aUsec;
}


inline Timestamp::Timestamp(const Timestamp &timestamp)
	:mUsec(timestamp.mUsec)
{
}


inline Timestamp& Timestamp::operator=(const Timestamp &timestamp)
{
	this->mUsec = timestamp.mUsec;
	return *this;
}


inline uint64_t Timestamp::sec() const
{
	return static_cast<uint64_t>(this->mUsec/TIME_USEC);
}


inline uint64_t Timestamp::usec() const
{
	return this->mUsec;
}


inline uint64_t Timestamp::msec() const
{
	return static_cast<uint64_t>(this->mUsec/TIME_MSEC);
}


inline Timestamp Timestamp::now()
{
	return Timestamp();
}


inline Timestamp& Timestamp::operator+=(const Timestamp &timestamp)
{
	this->mUsec += timestamp.mUsec;
	return *this;
}


inline Timestamp& Timestamp::operator-=(const Timestamp &timestamp)
{
	if (this->mUsec < timestamp.mUsec) {
		this->mUsec = 0;
	}
	else {
		this->mUsec -= timestamp.mUsec;
	}
	return *this;
}


inline Timestamp operator+(const Timestamp &left, const Timestamp &right)
{
	Timestamp ts = Timestamp(left);
	ts.mUsec += right.mUsec;
	return ts;
}


inline Timestamp operator-(const Timestamp &left, const Timestamp &right)
{
	if (left.mUsec <= right.mUsec) {
		return Timestamp(0);
	}
	Timestamp ts = Timestamp(left);
	ts.mUsec -= right.mUsec;
	return ts;
}

inline bool operator>(const Timestamp &left, const Timestamp &right)
{
	return left.mUsec > right.mUsec;
}


inline bool operator>=(const Timestamp &left, const Timestamp &right)
{
	return left.mUsec >= right.mUsec;
}


inline bool operator<(const Timestamp &left, const Timestamp &right)
{
	return left.mUsec < right.mUsec;
}


inline bool operator<=(const Timestamp &left, const Timestamp &right)
{
	return left.mUsec <= right.mUsec;
}


inline bool operator==(const Timestamp &left, const Timestamp &right)
{
	return left.mUsec == right.mUsec;
}

inline bool operator!=(const Timestamp &left, const Timestamp &right)
{
	return !(left == right);
}

}

#endif
