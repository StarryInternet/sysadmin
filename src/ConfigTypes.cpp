
#include "ConfigTypes.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "decibel/ToString.h"
#include <boost/algorithm/string.hpp>

#define VECTOR_STRING_SPECIALIZER(TYPE, PREPENDER, POSTPENDER)                              \
    template <>                                                                             \
    std::string ValStringer::operator()(const ConfigType<std::vector<TYPE>>& operand) const \
    {                                                                                       \
        std::stringstream out;                                                              \
        for (size_t i = 0; i < operand.mValue.size(); i++)                                  \
        {                                                                                   \
            out << PREPENDER << operand.mValue[i] << POSTPENDER;                            \
            if (i != operand.mValue.size() - 1)                                             \
            {                                                                               \
                out << ",";                                                                 \
            }                                                                               \
        }                                                                                   \
        return out.str();                                                                   \
    }

class ValStringer : public boost::static_visitor<std::string>
{
public:
    template <typename T>
    std::string operator()(const T& operand) const
    {
        return decibel::ToString(operand.mValue);
    }
};

template <>
std::string ValStringer::operator()(const ConfigType<bool>& operand) const
{
    std::stringstream out;
    out << std::boolalpha << operand.mValue;
    return out.str();
}

VECTOR_STRING_SPECIALIZER(int32_t, "", "");
VECTOR_STRING_SPECIALIZER(std::string, "", "");
VECTOR_STRING_SPECIALIZER(bool, std::boolalpha, std::noboolalpha);

ConfigValue::ConfigValue(ValueType value)
  : mValue(value)
  , mEmpty(false)
{
}

ConfigValue::ConfigValue()
  : mValue(ConfigType<int32_t>(0))
  , mEmpty(true)
{
}

bool ConfigValue::Empty() const
{
    return mEmpty;
}

std::string ConfigValue::ToString() const
{
    if (mEmpty)
    {
        return "NULL";
    }
    return boost::apply_visitor(ValStringer(), mValue);
}

std::string ConfigValue::TypeToString() const
{
    if (mEmpty)
    {
        return "NULL";
    }
    switch (mValue.which())
    {
        case 0:
            return "int32";
        case 1:
            return "string";
        case 2:
            return "bool";
        case 3:
            return "int32list";
        case 4:
            return "stringlist";
        case 5:
            return "boollist";
    }
    return "unknown type";
}

const std::string ConfigKey::DELIMITER = ".";

ConfigKey::ConfigKey(const std::vector<std::string>& subkeys)
  : mSubKeys(subkeys)
{
}

ConfigKey::ConfigKey(const std::string& key)
  : mSubKeys()
{
    if (key.size() > 0)
    {
        boost::split(mSubKeys, key, boost::is_any_of(DELIMITER));
    }
}

std::string ConfigKey::ToString() const
{
    std::stringstream output;

    for (auto ci = mSubKeys.begin(); ci != mSubKeys.end(); ci++)
    {
        output << *ci;
        if (ci != mSubKeys.end() - 1)
        {
            output << DELIMITER;
        }
    }

    return output.str();
}

ConfigKey ConfigKey::Init() const
{
    if (mSubKeys.size() > 0)
    {
        return ConfigKey((std::vector<std::string>(mSubKeys.begin(), mSubKeys.end() - 1)));
    }
    return ConfigKey((std::vector<std::string>()));
}

ConfigKey ConfigKey::Last() const
{
    if (mSubKeys.size() > 0)
    {
        return ConfigKey((std::vector<std::string>({mSubKeys.back()})));
    }
    return ConfigKey((std::vector<std::string>()));
}

ConfigKey ConfigKey::Head() const
{
    if (mSubKeys.size() > 0)
    {
        return ConfigKey(mSubKeys.front());
    }
    return ConfigKey((std::vector<std::string>()));
}

ConfigKey ConfigKey::Tail() const
{
    if (mSubKeys.size() > 1)
    {
        return ConfigKey((std::vector<std::string>(mSubKeys.begin() + 1, mSubKeys.end())));
    }
    return ConfigKey((std::vector<std::string>()));
}

bool ConfigKey::SubkeyMatches(const ConfigKey& key) const
{
    BOOST_ASSERT(mSubKeys.size() > 0 && key.mSubKeys.size() > 0);
    auto me = mSubKeys.begin();
    auto them = key.mSubKeys.begin();
    for (; me != mSubKeys.end() && them != key.mSubKeys.end(); me++, them++)
    {
        if (!(*me == *them || (*me == "*" || *them == "*")))
        {
            return false;
        }
    }
    if (me == mSubKeys.end() && them != key.mSubKeys.end() &&
        mSubKeys.back() != "*")
    {
        return false;
    }
    return true;
}

bool ConfigKey::IsWildcard() const
{
    return std::find(mSubKeys.begin(), mSubKeys.end(), "*") != mSubKeys.end();
}

std::vector<std::string> ConfigKey::KeyParts() const
{
    return mSubKeys;
}

ConfigPair::ConfigPair(const Key& key, ConfigValue::ValueType value)
  : mKey(key)
  , mValue(ConfigValue(value))
{
}

ConfigPair::ConfigPair(const std::string& key, ConfigValue::ValueType value)
  : mKey(key)
  , mValue(ConfigValue(value))
{
}

ConfigPair::ConfigPair(const Key& key, ConfigValue value)
  : mKey(key)
  , mValue(value)
{
}

ConfigPair::ConfigPair(const std::string& key, ConfigValue value)
  : mKey(key)
  , mValue(value)
{
}

ConfigPair::ConfigPair(const Key& key)
  : mKey(key)
  , mValue()
{
}

ConfigPair::ConfigPair(const std::string& key)
  : mKey(key)
  , mValue()
{
}

ConfigPair::Key ConfigPair::GetKey() const
{
    return mKey;
}

ConfigValue ConfigPair::GetValue() const
{
    return mValue;
}

std::string ConfigPair::ToString() const
{
    return mKey.ToString() + ": " + mValue.ToString();
}

boost::optional<ConfigPair> UnwrapSinglePair(const ConfigPairMap& values)
{
    if (values.size() > 0)
    {
        for (const auto& iter : values)
        {
            return boost::make_optional(iter.second);
        }
    }
    return boost::none;
}
