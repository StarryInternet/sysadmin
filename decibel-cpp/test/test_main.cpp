#include <google/protobuf/stubs/common.h>
#include <gtest/gtest.h>
#include <log4cxx/propertyconfigurator.h>

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
    auto output = RUN_ALL_TESTS();
    google::protobuf::ShutdownProtobufLibrary();
    return output;
}
