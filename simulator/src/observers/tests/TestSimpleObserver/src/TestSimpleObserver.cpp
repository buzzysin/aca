#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TkArch/Events.h>

class MockSimpleObserver : public SimpleObserver
{
public:
  MOCK_METHOD(void, next, (void *value), (override));
  MOCK_METHOD(void, error, (const std::exception &e), (override));
  MOCK_METHOD(void, complete, (), (override));
};

class TestSimpleObserver : public ::testing::Test
{
protected:
  TestSimpleObserver();
  virtual ~TestSimpleObserver();

  virtual void SetUp();
  virtual void TearDown();

  // Objects declared here can be used by all tests in the test case for Foo.
};

TestSimpleObserver::TestSimpleObserver() {
  // You can do set-up work for each test here.
}

TestSimpleObserver::~TestSimpleObserver() {
  // You can do clean-up work that doesn't throw exceptions here.
}

void TestSimpleObserver::SetUp() {
  // Code here will be called immediately after the constructor (right
  // before each test).
}

void TestSimpleObserver::TearDown() {
  // Code here will be called immediately after each test (right
  // before the destructor).
}

TEST_F(TestSimpleObserver, TestNext) {
  SimpleObserver observer([](void *value) { EXPECT_EQ(value, nullptr); });

  observer.next(nullptr);
}

TEST_F(TestSimpleObserver, TestError) {
  SimpleObserver observer([](const std::exception &e) {
    std::string error = e.what();
    EXPECT_EQ(error, "Error");
  });

  observer.error(std::runtime_error("Error"));
}

TEST_F(TestSimpleObserver, TestComplete) {
  bool complete = false;

  SimpleObserver observer([&complete]() { complete = true; });

  observer.complete();
}

// Path:
// simulator/src/observers/tests/TestSimpleObserver/src/TestSimpleObserver.h