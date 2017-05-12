#pragma once

#include <boost/optional.hpp>
#include <folly/futures/Future.h>
#include <google/protobuf/message.h>

#include <functional>
#include <string>
#include <unordered_map>

namespace decibel
{
boost::optional<std::string>
GetOneofFieldName(const google::protobuf::Message& message,
                  const std::string& oneofFieldName);

class ProtobufError : public std::runtime_error
{
public:
    ProtobufError(const std::string& str) : std::runtime_error(str)
    {
    }
};

template <typename MessageType, typename ReturnType, typename... Args>
class OneofDispatcher
{
public:
    using ReturnTypeImpl = typename folly::Unit::Lift<ReturnType>::type;
    using FutureReturnType = typename folly::Future<ReturnTypeImpl>;

    typedef std::function<FutureReturnType(const MessageType&, Args...)>
        SubmessageHandler;

public:
    void RegisterHandler(const std::string& field, SubmessageHandler&& handler)
    {
        mHandlers.emplace(field, handler);
    }

    FutureReturnType Dispatch(const MessageType& message,
                              const std::string& oneofFieldName,
                              Args... args)
    {
        auto maybeName = GetOneofFieldName(message, oneofFieldName);
        if (!maybeName)
        {
            return folly::makeFuture<ReturnTypeImpl>(
                ProtobufError("Could not find set oneof field"));
        }
        auto& name = maybeName.get();
        auto handlerIt = mHandlers.find(name);
        if (handlerIt == mHandlers.end())
        {
            return folly::makeFuture<ReturnTypeImpl>(
                ProtobufError("No handler for oneof found"));
        }

        return folly::makeFutureWith([&handlerIt, &message, &args...] {
            return handlerIt->second(message, args...);
        });
    }

private:
    std::unordered_map<std::string, SubmessageHandler> mHandlers;
};

template <typename T>
struct ProtobufSetter
{
    // void SetMessageField(google::protobuf::Message* pMessage, const
    // std::string& field, const T& value);
    // T GetMessageField(const google::protobuf::Message& message, const
    // std::string& field);
};

#define DEFINE_PROTOBUF_FIELD_MAPPING(TYPE, PB_TYPENAME)                       \
    template <>                                                                \
    struct ProtobufSetter<TYPE>                                                \
    {                                                                          \
        static void SetMessageField(google::protobuf::Message* pMessage,       \
                                    const std::string& field,                  \
                                    const TYPE& value)                         \
        {                                                                      \
            auto pReflection = pMessage->GetReflection();                      \
            auto pDescriptor = pMessage->GetDescriptor();                      \
            auto pFieldDescriptor = pDescriptor->FindFieldByName(field);       \
            pReflection->Set##PB_TYPENAME(pMessage, pFieldDescriptor, value);  \
        }                                                                      \
                                                                               \
        static TYPE GetMessageField(const google::protobuf::Message& message,  \
                                    const std::string& field)                  \
        {                                                                      \
            auto pReflection = message.GetReflection();                        \
            auto pDescriptor = message.GetDescriptor();                        \
            auto pFieldDescriptor = pDescriptor->FindFieldByName(field);       \
            return pReflection->Get##PB_TYPENAME(message, pFieldDescriptor);   \
        }                                                                      \
    };

DEFINE_PROTOBUF_FIELD_MAPPING(int32_t, Int32);
DEFINE_PROTOBUF_FIELD_MAPPING(int64_t, Int64);
DEFINE_PROTOBUF_FIELD_MAPPING(uint32_t, UInt32);
DEFINE_PROTOBUF_FIELD_MAPPING(uint64_t, UInt64);
DEFINE_PROTOBUF_FIELD_MAPPING(float, Float);
DEFINE_PROTOBUF_FIELD_MAPPING(double, Double);
DEFINE_PROTOBUF_FIELD_MAPPING(bool, Bool);
DEFINE_PROTOBUF_FIELD_MAPPING(std::string, String);

template <typename T>
void SetMessageField(google::protobuf::Message* pMessage,
                     const std::string& field,
                     const T& value)
{
    ProtobufSetter<T>::SetMessageField(pMessage, field, value);
}

template <typename T>
T GetMessageField(const google::protobuf::Message& pMessage,
                  const std::string& field)
{
    return ProtobufSetter<T>::GetMessageField(pMessage, field);
}

template <typename T>
std::vector<T>
UnpackRepeated(const google::protobuf::RepeatedField<T>& repeated)
{
    std::vector<T> output;
    for (const auto& v : repeated)
    {
        output.push_back(v);
    }
    return output;
}

template <typename T>
std::vector<T>
UnpackRepeated(const google::protobuf::RepeatedPtrField<T>& repeated)
{
    std::vector<T> output;
    for (const auto& v : repeated)
    {
        output.push_back(v);
    }
    return output;
}

} // decibel
