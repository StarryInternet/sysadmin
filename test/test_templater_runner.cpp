#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <log4cxx/logger.h>
#include <fstream>
#include <string>

#include "TemplaterRunner.h"
#include "decibel/messaging/Reactor.h"

namespace dm = decibel::messaging;

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_templater_runner"));
}

TEST(TemplaterRunnering, Basic)
{
    dm::Reactor r;
    std::vector<std::string> args = {"", ""};
    TemplaterRunner runner(&r, "./argdumper.py", "template", "dest", args);
    int calledback = 0;
    runner.Run({"key", "key2", "key3"}).thenValue([&r, &calledback](auto /*unused*/)
    {
        calledback++;
        r.Stop();
    }).thenError(
        folly::tag_t<ExternalRunnerError>{},
        [](const auto&)
        {
            FAIL() << "Should not have errored out";
        });
    r.Start();
    ASSERT_EQ(1, calledback);
}

TEST(TemplaterRunnering, ActualOutput)
{
    dm::Reactor r;
    std::vector<std::string> args = {"wl50", "main"};
    TemplaterRunner runner(&r, "./argdumper.py", "template", "dest", args);
    int calledback = 0;
    runner.Run({"key", "key2", "key3"}).thenValue([&r, &calledback](auto /*unused*/)
    {
        calledback++;
        r.Stop();
    }).thenError(
        folly::tag_t<ExternalRunnerError>{},
        [](const auto& e)
        {
            FAIL() << "Should not have errored out: " << e.what();
        });
    r.Start();
    ASSERT_EQ(1, calledback);

    std::ifstream ifs("argdumper");
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ASSERT_STREQ("template dest --template-args wl50 main --committed key key2 key3\n", content.c_str());
}

TEST(TemplaterRunnering, FailedOutput)
{
    dm::Reactor r;
    std::vector<std::string> args = {"", ""};

    // argdumper knows to exit with a bad status when receiving "failme"
    TemplaterRunner runner(&r, "./argdumper.py", "failme", "dest", args);
    int calledback = 0;
    runner.Run({"key", "key2", "key3"}).thenValue([](auto /*unused*/)
    {
        FAIL() << "Should not have succeeded";
    }).thenError(
        folly::tag_t<ExternalRunnerError>{},
        [&r, &calledback](const auto&)
        {
            calledback++;
            r.Stop();
        });
    r.Start();
    ASSERT_EQ(1, calledback);
}
