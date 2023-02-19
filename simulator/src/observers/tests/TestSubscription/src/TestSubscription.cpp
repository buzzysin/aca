#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TkArch/Events.h>

class MockSubscription : public Subscription
{
public:
  MOCK_METHOD(void, add, (Subscription *), ());
  MOCK_METHOD(void, add, (std::function<void()>), ());

  MOCK_METHOD(bool, closed, (), (const, override));
  MOCK_METHOD(void, unsubscribe, (), (override));
  MOCK_METHOD(void, teardown, (), (override));
};

class TestSubscription : public ::testing::Test
{
protected:
  TestSubscription();
  virtual ~TestSubscription();

  virtual void SetUp();
  virtual void TearDown();

  // Objects declared here can be used by all tests in the test case for Foo.
};

TestSubscription::TestSubscription() {
  // You can do set-up work for each test here.
}

TestSubscription::~TestSubscription() {
  // You can do clean-up work that doesn't throw exceptions here.
}

void TestSubscription::SetUp() {
  // Code here will be called immediately after the constructor (right
  // before each test).
}

void TestSubscription::TearDown() {
  // Code here will be called immediately after each test (right
  // before the destructor).
}

TEST_F(TestSubscription, ItCanUnsubscribe) {
  //
}

TEST_F(TestSubscription, ItClosesAfterUnsubscribe) {
  Subscription subscription;

  EXPECT_FALSE(subscription.closed());

  subscription.unsubscribe();

  EXPECT_TRUE(subscription.closed());
}

TEST_F(TestSubscription, ItCallsTheProvidedTeardown) {
  Subscription subscription;

  bool teardown = false;

  subscription.add([&teardown] { teardown = true; });

  EXPECT_FALSE(teardown);

  subscription.unsubscribe();

  EXPECT_TRUE(teardown);
}

TEST_F(TestSubscription, ItDoesNotCallTheProvidedTeardownTwice) {
  Subscription subscription;

  int closeAttempts = 0;

  subscription.add([&closeAttempts] { closeAttempts++; });

  subscription.unsubscribe();

  EXPECT_EQ(1, closeAttempts);

  subscription.unsubscribe();

  EXPECT_EQ(1, closeAttempts);
}

TEST_F(TestSubscription, ItCallsTheProvidedTeardownWhenTeardownIsCalled) {
  Subscription subscription;
  
  bool teardown = false;

  subscription.add([&teardown] { teardown = true; });

  EXPECT_FALSE(teardown);

  subscription.teardown();

  EXPECT_TRUE(teardown);
}

TEST_F(TestSubscription, ItUnsubscribesWhenTeardownIsCalled) {
  Subscription subscription;

  EXPECT_FALSE(subscription.closed());

  subscription.teardown();

  EXPECT_TRUE(subscription.closed());
}
