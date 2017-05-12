#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace decibel
{
namespace messaging
{

template <typename LengthT>
class FramedBuffer
{
    static_assert(
        std::is_integral<LengthT>::value,
        "FramedBuffer length type must be integral"
    );

public:
    typedef LengthT LengthType;

    FramedBuffer()
        : mFullSize(0)
        , mSizeBuffer()
        , mBytes()
    {
    }

    LengthType AddData(const char* bytes, LengthType size)
    {
        LengthType bytesUsed = 0;
        if (mSizeBuffer.size() < sizeof(LengthType))
        {
            size_t sizeByteCount = std::min(
                (size_t)size,
                sizeof(LengthType) - mSizeBuffer.size()
            );
            mSizeBuffer.insert(
                mSizeBuffer.end(),
                bytes,
                bytes + sizeByteCount
            );
            bytesUsed += sizeByteCount;
            if (mSizeBuffer.size() == sizeof(LengthType))
            {
                memcpy(&mFullSize, mSizeBuffer.data(), sizeof(LengthType));
            }
            else
            {
                // Don't continue if we don't have size set yet
                return bytesUsed;
            }
        }
        size_t byteCount = std::min(
            (size_t)size - bytesUsed,
            mFullSize - mBytes.size()
        );
        mBytes.insert(
            mBytes.end(),
            bytes + bytesUsed,
            bytes + bytesUsed + byteCount
        );
        bytesUsed += byteCount;
        return bytesUsed;
    }

    bool Ready() const
    {
        return mSizeBuffer.size() == sizeof(LengthType) &&
            mFullSize == mBytes.size();
    }

    const char* Data() const
    {
        return mBytes.data();
    }

    size_t CurrentSize() const
    {
        return mBytes.size();
    }

    size_t Size() const
    {
        return mFullSize;
    }

private:
    LengthType mFullSize;
    std::vector<char> mSizeBuffer;
    std::vector<char> mBytes;
};
}
}
