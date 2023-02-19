#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TkArch/Events.h>

template <typename T, typename SourceT = T>
class MockSubscriber : public Subscriber<T, SourceT>
{
public:
  MOCK_METHOD(void, next, (const T &), (override));
  MOCK_METHOD(void, error, (const std::exception &), (override));
  MOCK_METHOD(void, complete, (), (override));

  MOCK_METHOD(void, unsubscribe, (), (override));
};

class TestSubscriber : public ::testing::Test
{
protected:
  TestSubscriber();
  virtual ~TestSubscriber();

  virtual void SetUp();
  virtual void TearDown();

  // Objects declared here can be used by all tests in the test case for Foo.
};

TestSubscriber::TestSubscriber() {
  // You can do set-up work for each test here.
}

TestSubscriber::~TestSubscriber() {
  // You can do clean-up work that doesn't throw exceptions here.
}

void TestSubscriber::SetUp() {
  // Code here will be called immediately after the constructor (right
  // before each test).
}

void TestSubscriber::TearDown() {
  // Code here will be called immediately after each test (right
  // before the destructor).
}

TEST_F(TestSubscriber, ItPushesValuesViaNext) {
  //
}

TEST_F(TestSubscriber, ItPushesErrorsViaError) {
  //
}

TEST_F(TestSubscriber, ItCompletesViaComplete) {
  //
}

TEST_F(TestSubscriber, ItCanUnsubscribe) {
  //
}