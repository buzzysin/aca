#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TkArch/Events.h>

template <typename T>
class MockSimpleTypedObserver : public SimpleTypedObserver<T>
{
public:
  MOCK_METHOD(void, next, (const T &), (override));
  MOCK_METHOD(void, error, (const std::exception &e), (override));
  MOCK_METHOD(void, complete, (), (override));
};

class TestSimpleTypedObserver : public ::testing::Test
{
protected:
  TestSimpleTypedObserver();
  virtual ~TestSimpleTypedObserver();

  virtual void SetUp();
  virtual void TearDown();

  // Objects declared here can be used by all tests in the test case for Foo.
};

TestSimpleTypedObserver::TestSimpleTypedObserver() {
  // You can do set-up work for each test here.
}

TestSimpleTypedObserver::~TestSimpleTypedObserver() {
  // You can do clean-up work that doesn't throw exceptions here.
}

void TestSimpleTypedObserver::SetUp() {
  // Code here will be called immediately after the constructor (right
  // before each test).
}

void TestSimpleTypedObserver::TearDown() {
  // Code here will be called immediately after each test (right
  // before the destructor).
}

TEST_F(TestSimpleTypedObserver, TestNext) {
  SimpleTypedObserver<int> *observer = new SimpleTypedObserver<int>(
      [](const int &value) { EXPECT_EQ(value, 1); });

  observer->next(1);
}

TEST_F(TestSimpleTypedObserver, TestError) {

  SimpleTypedObserver<int> *observer =
      new SimpleTypedObserver<int>([](const std::exception &e) {
        std::string error = e.what();
        EXPECT_EQ(error, "Error");
      });

  observer->error(std::runtime_error("Error"));
}

TEST_F(TestSimpleTypedObserver, TestComplete) {
  bool completed = false;

  SimpleTypedObserver<int> *observer =
      new SimpleTypedObserver<int>([&completed]() { completed = true; });

  observer->complete();

  EXPECT_TRUE(completed);
}

// Path:
// simulator/src/observers/tests/TestSimpleTypedObserver/src/TestSimpleTypedObserver.h