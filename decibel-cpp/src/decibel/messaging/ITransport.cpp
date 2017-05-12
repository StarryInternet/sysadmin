#include "decibel/messaging/ITransport.h"

namespace decibel
{
namespace messaging
{
ITransport::~ITransport()
{
}

void ITransport::OnDisconnect(std::function<void(int)> callback)
{
    mOnDisconnectCallback = callback;
}

} // messaging
} // decibel
