
#include "ProtobufTranslator.h"

#include "decibel/Protobuf.h"
#include <log4cxx/logger.h>

#define PROTO_LIST_TRANSLATE(PROTO_TYPE, LIST, VALUE)              \
    for (int i = 0; i < VALUE.PROTO_TYPE##list().list_size(); i++) \
    {                                                              \
        LIST.push_back(VALUE.PROTO_TYPE##list().list(i));          \
    }

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("ProtobufTranslator"));
}

ProtobufTranslator::ProtobufTranslator()
{

}

ProtobufTranslator::~ProtobufTranslator()
{

}

void ProtobufTranslator::CopyConfigValue(const ConfigValue& from, sysadminctl::ConfigValue* to)
{
    if (from.Is<int32_t>())
    {
        to->set_int32val(from.Unpack<int32_t>());
    }
    else if (from.Is<std::string>())
    {
        to->set_strval(from.Unpack<std::string>());
    }
    else if (from.Is<bool>())
    {
        to->set_boolval(from.Unpack<bool>());
    }
    else if (from.Is<std::vector<int32_t>>())
    {
        std::vector<int32_t> list = from.Unpack<std::vector<int32_t>>();
        auto proto_list = to->mutable_int32list();
        for (const auto& item : list)
        {
            proto_list->add_list(item);
        }
    }
    else if (from.Is<std::vector<bool>>())
    {
        std::vector<bool> list = from.Unpack<std::vector<bool>>();
        auto proto_list = to->mutable_boollist();
        for (const auto& item : list)
        {
            proto_list->add_list(item);
        }
    }
    else if (from.Is<std::vector<std::string>>())
    {
        std::vector<std::string> list = from.Unpack<std::vector<std::string>>();
        auto proto_list = to->mutable_stringlist();
        for (const auto& item : list)
        {
            proto_list->add_list(item);
        }
    }
    else
    {
        LOG4CXX_ERROR(spLogger, "Type of value could not be determined: " << from.ToString());
    }
}

boost::optional<ConfigPair::Key> ProtobufTranslator::TranslateToGet(const sysadminctl::Command& message)
{
    boost::optional<ConfigPair::Key> maybeReturn;
    if (!message.has_get())
    {
        return maybeReturn;
    }
    maybeReturn = ConfigPair::Key(message.get().key());
    return maybeReturn;
}

boost::optional<ConfigPair::Key> ProtobufTranslator::TranslateToErase(const sysadminctl::Command& message)
{
    boost::optional<ConfigPair::Key> maybeReturn;
    if (!message.has_erase())
    {
        return maybeReturn;
    }
    maybeReturn = ConfigPair::Key(message.erase().key());
    return maybeReturn;
}

boost::optional<ConfigPair> ProtobufTranslator::TranslateToSet(const sysadminctl::Command& message)
{
    boost::optional<ConfigPair> maybeReturn;
    if (!message.has_set())
    {
        return maybeReturn;
    }
    if (!message.set().has_value())
    {
        return maybeReturn;
    }
    const auto& field = decibel::GetOneofFieldName(message.set().value(), "value");
    if (!field)
    {
        return maybeReturn;
    }
    const auto& value = message.set().value();
    const auto& fieldname = field.get();
    if (fieldname == "int32val")
    {
        maybeReturn = ConfigPair(message.set().key(), ConfigType<int32_t>(value.int32val()));
    }
    else if (fieldname == "strval")
    {
        maybeReturn = ConfigPair(message.set().key(), ConfigType<std::string>(value.strval()));
    }
    else if (fieldname == "boolval")
    {
        maybeReturn = ConfigPair(message.set().key(), ConfigType<bool>(value.boolval()));
    }
    else if (fieldname == "int32list")
    {
        std::vector<int32_t> list;
        PROTO_LIST_TRANSLATE(int32, list, value);
        maybeReturn = ConfigPair(message.set().key(), ConfigType<std::vector<int32_t>>(list));
    }
    else if (fieldname == "boollist")
    {
        std::vector<bool> list;
        PROTO_LIST_TRANSLATE(bool, list, value);
        maybeReturn = ConfigPair(message.set().key(), ConfigType<std::vector<bool>>(list));
    }
    else if (fieldname == "stringlist")
    {
        std::vector<std::string> list;
        PROTO_LIST_TRANSLATE(string, list, value);
        maybeReturn = ConfigPair(message.set().key(), ConfigType<std::vector<std::string>>(list));
    }
    else
    {
        LOG4CXX_ERROR(spLogger, "No types for set field: " << field.get());
    }

    return maybeReturn;
}

boost::optional<ConfigPair::Key> ProtobufTranslator::TranslateToBlame(const sysadminctl::Command& message)
{
    boost::optional<ConfigPair::Key> maybeReturn;
    if (!message.has_blame())
    {
        return maybeReturn;
    }
    maybeReturn = ConfigPair::Key(message.blame().key());
    return maybeReturn;
}

sysadminctl::Response ProtobufTranslator::TranslateFromGet(const std::vector<ConfigPair>& items)
{
    sysadminctl::Response output;
    sysadminctl::GetResponse* getOutput = output.mutable_get();
    if (items.size() == 0)
    {
        output.set_status(sysadminctl::StatusCode::KEY_NOT_FOUND);
        return output;
    }

    for (const auto& item : items)
    {
        // Emptied values "exist" in the database, but they never exist from the clients perspective
        if (item.GetValue().Empty())
        {
            continue;
        }

        const auto& value = item.GetValue();
        const auto& keystr = item.GetKey();
        auto mapitem = getOutput->add_kvs();
        mapitem->set_key(keystr.ToString());
        auto msgvalue = mapitem->mutable_value();
        CopyConfigValue(value, msgvalue);
    }

    return output;
}

sysadminctl::Response ProtobufTranslator::TranslateFromInFlight(const std::vector<ConfigPair>& items)
{
    sysadminctl::Response output;
    sysadminctl::InFlightResponse* subOutput = output.mutable_inflight();

    for (const auto& item : items)
    {
        const auto& value = item.GetValue();
        const auto& keystr = item.GetKey();
        auto mapitem = subOutput->add_kvs();
        mapitem->set_key(keystr.ToString());
        if (!item.GetValue().Empty())
        {
            auto msgvalue = mapitem->mutable_value();
            CopyConfigValue(value, msgvalue);
        }
    }

    return output;
}

boost::optional<CommitHistory::CommitId>
ProtobufTranslator::TranslateToRollback(const sysadminctl::Command& message)
{
    if (!message.has_rollback())
    {
        return boost::optional<CommitHistory::CommitId>();
    }
    CommitHistory::CommitId id = message.rollback().id();
    return boost::optional<CommitHistory::CommitId>(id);
}

sysadminctl::Response
ProtobufTranslator::TranslateFromBlame(const ConfigPair::Key& key,
                                       const std::vector<BlameDelta>& deltas)
{
    sysadminctl::Response output;
    sysadminctl::BlameResponse* blameOutput = output.mutable_blame();
    blameOutput->set_key(key.ToString());
    for (auto& delta : deltas)
    {
        sysadminctl::BlameEntry* entry = blameOutput->add_entries();
        entry->set_commit_id(delta.mCommitId);
        entry->set_commit_time(boost::posix_time::to_simple_string(delta.mCommitTime));

        sysadminctl::ConfigValue* val = entry->mutable_val();
        CopyConfigValue(delta.mDelta.second.GetValue(), val);
    }

    return output;
}
