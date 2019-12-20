#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <log4cxx/logger.h>
#include <fstream>
#include <string>

#include "ServiceScriptRunner.h"
#include "decibel/messaging/Reactor.h"

namespace dm = decibel::messaging;

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_service_script_runner"));
}

TEST(ServiceScriptRunnering, Basic)
{
    dm::Reactor r;
    ServiceScriptRunner runner(&r, {"./argdumper.py"});
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

TEST(ServiceScriptRunnering, ActualOutput)
{
    dm::Reactor r;
    ServiceScriptRunner runner(&r, {"./argdumper.py"});
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

    std::ifstream ifs("argdumper");
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ASSERT_STREQ("key key2 key3\n", content.c_str());
}

TEST(ServiceScriptRunnering, WithServiceArgs)
{
    dm::Reactor r;
    ServiceScriptRunner runner(&r, {"./argdumper.py", "key0", "more", "args"});
    int calledback = 0;
    runner.Run({}).thenValue([&r, &calledback](auto /*unused*/)
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

    std::ifstream ifs("argdumper");
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ASSERT_STREQ("key0 more args\n", content.c_str());
}

TEST(ServiceScriptRunnering, FailedOutput)
{
    dm::Reactor r;
    ServiceScriptRunner runner(&r, {"./argdumper.py"});
    int calledback = 0;
    // argdumper knows to exit with a bad status when receiving "failme" as it's first argument
    runner.Run({"failme", "key2", "key3"}).thenValue([](auto /*unused*/)
    {
        FAIL() << "Should have errored out";
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

TEST(ServiceScriptRunnering, Detaching)
{
    dm::Reactor r;
    ServiceScriptRunner runner(&r, {"./argdumper.py"}, HookOptions::RunOptions::DETACH);
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

TEST(ServiceScriptRunnering, Ignoring)
{
    dm::Reactor r;
    ServiceScriptRunner runner(&r, {"./argdumper.py"}, HookOptions::RunOptions::IGNORE);
    int calledback = 0;
    runner.Run({"failme", "key2", "key3"}).thenValue([&r, &calledback](auto /*unused*/)
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
