#ifndef UNCOPYABLE_H
#define UNCOPYABLE_H

namespace snf
{

class Uncopyable 
{
protected:
	Uncopyable() {}
	~Uncopyable() {}
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator=(const Uncopyable&) = delete;
};

}

#endif
