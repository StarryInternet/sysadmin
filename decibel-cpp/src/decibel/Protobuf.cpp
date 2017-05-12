#include "decibel/Protobuf.h"

namespace decibel
{
boost::optional<std::string>
GetOneofFieldName(const google::protobuf::Message& message,
                  const std::string& oneofFieldName)
{
    boost::optional<std::string> maybeReturn;
    auto pMessageReflection = message.GetReflection();
    if (!pMessageReflection)
    {
        return maybeReturn;
    }
    auto pMessageDescriptor = message.GetDescriptor();
    auto pOneofDescriptor = pMessageDescriptor->FindOneofByName(oneofFieldName);
    if (!pOneofDescriptor)
    {
        return maybeReturn;
    }
    auto pPayloadFieldDescriptor =
        pMessageReflection->GetOneofFieldDescriptor(message, pOneofDescriptor);
    if (!pPayloadFieldDescriptor)
    {
        return maybeReturn;
    }
    maybeReturn = pPayloadFieldDescriptor->name();
    return maybeReturn;
}

} // decibel
