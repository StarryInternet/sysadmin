
#pragma once

#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <vector>
#include <unordered_map>

template <typename T>
class ConfigType
{
public:
    ConfigType(T val)
      : mValue(val)
    {
    }

    template <typename P>
    friend bool operator==(const ConfigType<P>& lhs, const ConfigType<P> rhs);
    template <typename F>
    friend bool operator!=(const ConfigType<F>& lhs, const ConfigType<F> rhs);

    T mValue;
};

template <typename T>
inline bool operator==(const ConfigType<T>& lhs, const ConfigType<T> rhs)
{
    return lhs.mValue == rhs.mValue;
}

template <typename T>
inline bool operator!=(const ConfigType<T>& lhs, const ConfigType<T> rhs)
{
    return !(lhs == rhs);
}

class ConfigValue
{
public:
    typedef boost::variant<ConfigType<int32_t>,
                           ConfigType<std::string>,
                           ConfigType<bool>,
                           ConfigType<std::vector<int32_t>>,
                           ConfigType<std::vector<std::string>>,
                           ConfigType<std::vector<bool>>> ValueType;

public:
    ConfigValue(ValueType value);

    ConfigValue();

    bool Empty() const;

    std::string ToString() const;

    std::string TypeToString() const;

    template <typename T>
    T Unpack() const
    {
        BOOST_ASSERT_MSG(!mEmpty, "Can't dereference Empty Value");
        try
        {
            return boost::get<ConfigType<T>>(mValue).mValue;
        }
        catch (const boost::bad_get& ex)
        {
            BOOST_ASSERT_MSG(false, "Requested incorrect type from variant");
            return T();
        }
    }

    template <typename T>
    ConfigType<T> ToConfigType() const
    {
        BOOST_ASSERT_MSG(!mEmpty, "Can't dereference Empty Value");
        try
        {
            return boost::get<ConfigType<T>>(mValue);
        }
        catch (const boost::bad_get& ex)
        {
            BOOST_ASSERT_MSG(false, "Requested incorrect type from variant");
            return ConfigType<T>({});
        }
    }

    template <typename T>
    bool Is() const
    {
        if (mEmpty)
        {
            return false;
        }
        try
        {
            boost::get<ConfigType<T>>(mValue);
            return true;
        }
        catch (const boost::bad_get& ex)
        {
            return false;
        }
    }

    static ConfigValue None()
    {
        static ConfigValue instance;
        return instance;
    }

    inline bool TypeCheck(const ConfigValue& comp) const
    {
        return mValue.which() == comp.mValue.which();
    }

    friend bool operator==(const ConfigValue& lhs, const ConfigValue& rhs);
    friend bool operator!=(const ConfigValue& lhs, const ConfigValue& rhs);

private:
    ValueType mValue;
    bool mEmpty;
};

inline bool operator==(const ConfigValue& lhs, const ConfigValue& rhs)
{
    if (lhs.mEmpty && rhs.mEmpty)
    {
        return true;
    }
    return lhs.mValue == rhs.mValue;
}

inline bool operator!=(const ConfigValue& lhs, const ConfigValue& rhs)
{
    return !(lhs == rhs);
}

class ConfigKey
{
public:
    static const std::string DELIMITER;

private:
    ConfigKey(const std::vector<std::string>& subkeys);

public:
    ConfigKey(const std::string& key);

    std::string ToString() const;

    ConfigKey Init() const;

    ConfigKey Last() const;

    ConfigKey Head() const;

    ConfigKey Tail() const;

    bool SubkeyMatches(const ConfigKey& key) const;

    bool IsWildcard() const;

    std::vector<std::string> KeyParts() const;

    friend bool operator==(const ConfigKey& lhs, const ConfigKey& rhs);
    friend bool operator!=(const ConfigKey& lhs, const ConfigKey& rhs);
    friend bool operator<(const ConfigKey& lhs, const ConfigKey& rhs);
    friend bool operator>(const ConfigKey& lhs, const ConfigKey& rhs);

private:
    std::vector<std::string> mSubKeys;
};

class ConfigKeyHasher
{
public:
    inline size_t operator()(const ConfigKey& key) const
    {
        std::hash<std::string> hasher;
        return hasher(key.ToString());
    }
};

inline bool operator==(const ConfigKey& lhs, const ConfigKey& rhs)
{
    return lhs.mSubKeys == rhs.mSubKeys;
}

inline bool operator!=(const ConfigKey& lhs, const ConfigKey& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(const ConfigKey& lhs, const ConfigKey& rhs)
{
    return lhs.ToString() < rhs.ToString();
}

inline bool operator>(const ConfigKey& lhs, const ConfigKey& rhs)
{
    return lhs.ToString() > rhs.ToString();
}

class ConfigPair
{
public:
    typedef ConfigKey Key;

public:
    ConfigPair(const Key& key, ConfigValue::ValueType value);

    ConfigPair(const std::string& key, ConfigValue::ValueType value);

    ConfigPair(const Key& key, ConfigValue value);

    ConfigPair(const std::string& key, ConfigValue value);

    ConfigPair(const Key& key);

    ConfigPair(const std::string& key);

    Key GetKey() const;

    ConfigValue GetValue() const;

    std::string ToString() const;

    template <typename T>
    T UnpackValue() const
    {
        BOOST_ASSERT_MSG(!mValue.Empty(), "Can't dereference Empty Value");
        return mValue.Unpack<T>();
    }

    bool TypeCheckValues(const ConfigPair& comp) const
    {
        return mValue.TypeCheck(comp.GetValue());
    }

    friend bool operator==(const ConfigPair& lhs, const ConfigPair& rhs);
    friend bool operator!=(const ConfigPair& lhs, const ConfigPair& rhs);

private:
    Key mKey;
    ConfigValue mValue;
};

inline bool operator==(const ConfigPair& lhs, const ConfigPair& rhs)
{
    return lhs.mKey == rhs.mKey && lhs.mValue == rhs.mValue;
}

inline bool operator!=(const ConfigPair& lhs, const ConfigPair& rhs)
{
    return !(lhs == rhs);
}

typedef std::vector<ConfigPair> ConfigPairList;

typedef std::unordered_map<ConfigPair::Key, ConfigPair, ConfigKeyHasher> ConfigPairMap;

boost::optional<ConfigPair> UnwrapSinglePair(const ConfigPairMap& values);
