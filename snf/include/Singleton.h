#ifndef SINGLETON_H
#define SINGLETON_H
#include <pthread.h>
#include <assert.h>

namespace snf
{

template<typename T>
class Singleton
{
public:
	static T& instance();

	static void destroy();

private:
	Singleton() = default;

	~Singleton() = default;

	static pthread_once_t once;

	static T *value;

};

template<typename T>
T *Singleton<T>::value = NULL;

template<typename T>
pthread_once_t Singleton<T>::once = PTHREAD_ONCE_INIT;

template<typename T>
T& Singleton<T>::instance()
{
	pthread_once(&once, [](){value = new T();});
	assert(value != NULL);
	return *value;
}

template<typename T>
void Singleton<T>::destroy()
{
	delete value;
	value = NULL;
}

}

#endif
