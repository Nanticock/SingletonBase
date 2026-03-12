#include <gtest/gtest.h>
#include "SingletonBase.h" // Include the header for the SingletonBase implementation

TEST(SingletonBaseTest, InstanceCreation) {
    SingletonBase* instance1 = SingletonBase::getInstance();
    SingletonBase* instance2 = SingletonBase::getInstance();
    
    EXPECT_EQ(instance1, instance2); // Ensure both instances are the same
}

TEST(SingletonBaseTest, FunctionalityTest) {
    SingletonBase* instance = SingletonBase::getInstance();
    
    // Add tests for specific functionality of SingletonBase
    EXPECT_TRUE(instance->someFunction()); // Replace with actual function and expected result
}