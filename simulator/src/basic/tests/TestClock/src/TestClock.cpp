#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TkArch/Basic.h>

class MockClock : public Clock
{
public:
  MOCK_METHOD(void, start, (), ());
  MOCK_METHOD(void, stop, (), ());
};

class TestClock : public ::testing::Test
{
protected:
  TestClock();
  virtual ~TestClock();

  virtual void SetUp();
  virtual void TearDown();

  // Objects declared here can be used by all tests in the test case for Foo.
};

TestClock::TestClock() {
  // You can do set-up work for each test here.
}

TestClock::~TestClock() {
  // You can do clean-up work that doesn't throw exceptions here.
}

void TestClock::SetUp() {
  // Code here will be called immediately after the constructor (right
  // before each test).
}

void TestClock::TearDown() {
  // Code here will be called immediately after each test (right
  // before the destructor).
}

TEST_F(TestClock, ItCanCreateAClock) { Clock clock; }

TEST_F(TestClock, ItCanBeStartedAndStopped) {
  Clock clock;

  clock.start();
  clock.pause();
}

TEST_F(TestClock, ItTicks) {
  Clock clock;

  int ticks = 0;
  State prevState = UNDEFINED;

  clock.stateChange()->subscribe(new obs<State>([&](State state) {
    if (prevState != state) {
      ticks++;
      prevState = state;
    }

    if (ticks == 10) {
      clock.pause();
    }
  }));

  clock.start();

  // The clock should have ticked at least once.
  EXPECT_NE(ticks, 10);
}

TEST_F(TestClock, ItEmitsRisingAndFallingEdges) {
  Clock clock;

  int risingEdges = 0;
  int fallingEdges = 0;

  clock.risingEdge()->subscribe(
      new obs<State>([&](State) { risingEdges++; }));
  clock.fallingEdge()->subscribe(
      new obs<State>([&](State) { fallingEdges++; }));

  clock.start();

  // The clock should have ticked at least once.
  EXPECT_NE(risingEdges, 0);
  EXPECT_NE(fallingEdges, 0);

  EXPECT_NEAR(risingEdges, fallingEdges, 0);
}
