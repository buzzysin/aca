#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TkArch/Events.h>

template <typename T>
class MockObserver : public Observer<T>
{
public:
  MOCK_METHOD(void, next, (const T &), (override));
  MOCK_METHOD(void, error, (const std::exception &), (override));
  MOCK_METHOD(void, complete, (), (override));
};

class TestObserver : public ::testing::Test
{
protected:
  TestObserver();
  virtual ~TestObserver();

  virtual void SetUp();
  virtual void TearDown();

  // Objects declared here can be used by all tests in the test case for Foo.
};

TestObserver::TestObserver() {
  // You can do set-up work for each test here.
}

TestObserver::~TestObserver() {
  // You can do clean-up work that doesn't throw exceptions here.
}

void TestObserver::SetUp() {
  // Code here will be called immediately after the constructor (right
  // before each test).
}

void TestObserver::TearDown() {
  // Code here will be called immediately after each test (right
  // before the destructor).
}

TEST_F(TestObserver, ItInitilizesInAllConstructors) {
  Observer<int> o = obs<int>([](int i) { EXPECT_EQ(i, 1); });
  Observer<int> o2 =
      obs<int>([](int i) { EXPECT_EQ(i, 1); },
               [](const std::exception &e) { EXPECT_EQ(e.what(), "test"); });
  Observer<int> o3 =
      obs<int>([](int i) { EXPECT_EQ(i, 1); },
               [](const std::exception &e) { EXPECT_EQ(e.what(), "test"); },
               []() { EXPECT_TRUE(true); });
}

TEST_F(TestObserver, ItPushesValuesViaNext) {
  Observer<int> o = obs<int>([](int i) { EXPECT_EQ(i, 1); });

  o.next(1);
}

TEST_F(TestObserver, ItPushesErrorsViaError) {
  Observer<int> o = obs<int>([](const std::exception &e) {
    std::string s = e.what();

    EXPECT_EQ(s, "test");
  });

  o.error(std::runtime_error("test"));
}

TEST_F(TestObserver, ItCompletes) {
  bool complete = false;

  Observer<int> o = obs<int>([&]() {
    complete = true;
    EXPECT_TRUE(complete);
  });

  o.complete();
}

TEST_F(TestObserver, ItPushesMultipleValuesViaNext) {
  int i = 0;

  Observer<int> o = obs<int>([&](int j) {
    EXPECT_EQ(i, j);
    i++;
  });

  o.next(0);
  o.next(1);
  o.next(2);
  o.next(3);
  o.next(4);
  o.next(5);
  o.next(6);
  o.next(7);
  o.next(8);
  o.next(9);
}

TEST_F(TestObserver, ItPushesMultipleErrorsViaError) {
  int i = 0;

  Observer<int> o = obs<int>([&](const std::exception &e) {
    std::string s = e.what();

    EXPECT_EQ(s, "test");
    i++;
  });

  o.error(std::runtime_error("test"));
}

TEST_F(TestObserver, ItCompletesMultipleTimes) {
  int i = 0;

  Observer<int> o = obs<int>([&]() {
    i++;
    EXPECT_EQ(i, 1);
  });

  o.complete();
  i--;

  o.complete();
  i--;
  
  o.complete();
  i--;
}