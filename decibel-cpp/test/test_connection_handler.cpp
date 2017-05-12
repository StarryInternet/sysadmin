#include <boost/circular_buffer.hpp>
#include <gtest/gtest.h>

// This is a basic test to validate the circular buffer operations that are
// done in ConnectionHandler
TEST(TestCircularBuffer, BasicLogic)
{
    boost::circular_buffer<uint8_t> buf(4096);
    std::vector<uint8_t> bytes(1024);
    buf.insert(buf.end(), bytes.begin(), bytes.end());
    ASSERT_EQ(buf.size(), 1024);
    auto start = buf.begin();
    auto end = start + 512;
    buf.erase(start, end);
    ASSERT_EQ(buf.size(), 512);
}
