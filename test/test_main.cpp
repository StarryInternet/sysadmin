#include <log4cxx/propertyconfigurator.h>
#include <gtest/gtest.h>

struct GlobalFixture : public ::testing::Environment
{
    virtual ~GlobalFixture()
    {
    }

    virtual void SetUp()
    {
        SetupLogging();
    }

    void SetupLogging()
    {
        log4cxx::PropertyConfigurator::configure("log4cxx.config");
    }
};

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GlobalFixture);
    return RUN_ALL_TESTS();
}
