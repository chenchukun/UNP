#ifndef CALLBACK_H
#define CALLBACK_H
#include <memory>
#include <functional>
#include "Timestamp.h"

namespace snf
{

class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void(TcpConnectionPtr)> ConnectionCallback;

typedef std::function<void(TcpConnectionPtr, Timestamp)> MessageCallback;

typedef std::function<void(TcpConnectionPtr)> WriteCompleteCallback;

typedef std::function<void(TcpConnectionPtr)> ErrorCallback;


}

#endif
