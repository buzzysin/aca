#include <gtest/gtest.h>

#include <TkArch/Basic.h>

class TestRegister : public ::testing::Test
{
protected:
  TestRegister();
  virtual ~TestRegister();

  virtual void SetUp();
  virtual void TearDown();

  // Objects declared here can be used by all tests in the test case for Foo.
};

TestRegister::TestRegister() {
  // You can do set-up work for each test here.
}

TestRegister::~TestRegister() {
  // You can do clean-up work that doesn't throw exceptions here.
}

void TestRegister::SetUp() {
  // Code here will be called immediately after the constructor (right
  // before each test).
}

void TestRegister::TearDown() {
  // Code here will be called immediately after each test (right
  // before the destructor).
}

TEST_F(TestRegister, ItCanCreateARegister) {
  Clock clock;
  Register reg(&clock);
}

TEST_F(TestRegister, ItCanSetTheValue) {
  Clock clock;
  Register reg(&clock);

  // Clock is not ticking, so the value should not change
  reg.dataStore(0x0F);
  ASSERT_EQ(reg.dataLoad(), 0x00);

  clock.pause(); // We will manually cycle the clock

  // Register is not enabled, so the value should not change
  reg.dataStore(0xF0);
  ASSERT_EQ(reg.dataLoad(), 0x00);

  // Enable the register
  reg.enable();

  // The clock did not tick, so the value should not change
  reg.dataStore(0xFF);
  ASSERT_EQ(reg.dataLoad(), 0x00);

  // Cycle the clock
  clock.cycle();

  // The value should change
  ASSERT_EQ(reg.dataLoad(), 0xFF);
}

TEST_F(TestRegister, ItCanResetTheValue) {
  Clock clock;
  Register reg(&clock);

  // Enable the register
  reg.enable();

  // Set the value
  reg.dataStore(0xF0);

  // Cycle the clock
  clock.cycle();

  // The value should change
  ASSERT_EQ(reg.dataLoad(), 0xF0);

  // Reset the register
  reg.reset();

  // Cycle the clock
  clock.cycle();

  // The value should change
  ASSERT_EQ(reg.dataLoad(), 0x00);
}

TEST_F(TestRegister, ItIgnoresWritesWhenDisabled) {
  Clock clock;
  Register reg(&clock);

  // Set the value
  reg.dataStore(0xF0);

  // Cycle the clock
  clock.cycle();

  // The value should not change
  ASSERT_EQ(reg.dataLoad(), 0x00);

  // Enable the register
  reg.enable();

  // Cycle the clock
  clock.cycle();

  // The value should change
  ASSERT_EQ(reg.dataLoad(), 0xF0);
}

TEST_F(TestRegister, ItIgnoresWritesWhenReset) {
  Clock clock;
  Register reg(&clock);

  // Enable the register
  reg.enable();

  // Set the value
  reg.dataStore(0xF0);

  // Cycle the clock
  clock.cycle();

  // The value should change
  ASSERT_EQ(reg.dataLoad(), 0xF0);

  // Reset the register
  reg.reset();

  // Cycle the clock
  clock.cycle();

  // The value should change
  ASSERT_EQ(reg.dataLoad(), 0x00);

  // Set the value
  reg.dataStore(0xFF);

  // Cycle the clock
  clock.cycle();

  // The value should not change
  ASSERT_EQ(reg.dataLoad(), 0x00);
}

TEST_F(TestRegister, ItWritesWhenResetIsTurnedOff)
{
  Clock clock;
  Register reg(&clock);

  // Enable the register
  reg.enable();

  // Set the value
  reg.dataStore(0xF0);

  // Cycle the clock
  clock.cycle();

  // The value should change
  ASSERT_EQ(reg.dataLoad(), 0xF0);

  // Reset the register
  reg.reset();

  // Cycle the clock
  clock.cycle();

  // The value should change
  ASSERT_EQ(reg.dataLoad(), 0x00);

  // Turn off reset
  reg.pass();

  // Cycle the clock
  clock.cycle();

  // The value should change. Input is
  // still 0xF0, but reset is off, so
  // the value should change.
  ASSERT_EQ(reg.dataLoad(), 0xF0);
}