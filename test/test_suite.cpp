//
// Created by Chenfan Sun on 3/13/17.
//

#include "test_suite.h"

#include "gtest/gtest.h"

using std::cout;
using std::endl;

class TestEnvironment : public ::testing::Environment {
public:
    virtual void SetUp() {
        cout << "Start Testing Environment..." << endl;
    }

    virtual void TearDown() {
        cout << endl;
        cout << "End of the Tests." << endl;
    }
};

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new TestEnvironment);
    return RUN_ALL_TESTS();
}