#include "item.hpp"
#include "machine.hpp"

#include <gtest/gtest.h>

using namespace conveyor;

TEST(MachineTest, PushAndQueueSize) {
    Machine m(0, {3, 4});

    Item item0(0, 0);
    Item item1(1, 1);

    m.pushToQueue(&item0);
    EXPECT_EQ(m.getQueueSize(), 1);

    m.pushToQueue(&item1);
    EXPECT_EQ(m.getQueueSize(), 2);
}

TEST(MachineTest, CalculateWaitTime) {
    Machine m(0, {3, 5, 2});

    Item item0(0, 0);
    Item item1(1, 1);
    Item item2(2, 2);

    m.pushToQueue(&item0);
    m.pushToQueue(&item1);
    m.pushToQueue(&item2);

    EXPECT_EQ(m.calculateWaitTime(), 3 + 5 + 2);
}

TEST(MachineTest, StartProcessingAndIdle) {
    Machine m(0, {3, 4});

    Item item(0, 0);

    m.startProcessing(&item, 10);

    EXPECT_FALSE(m.isIdle(10));
    EXPECT_FALSE(m.isIdle(12));

    EXPECT_TRUE(m.isIdle(13));

    EXPECT_EQ(m.getBusyUntil(), 13);
    EXPECT_EQ(m.getCurrentItem(), &item);
}

TEST(MachineTest, PopNextIfReady) {
    Machine m(0, {3, 4});

    Item item0(0, 0);
    Item item1(1, 1);

    m.pushToQueue(&item0);
    m.pushToQueue(&item1);

    Item* next = m.popNextIfReady(0);

    ASSERT_NE(next, nullptr);
    EXPECT_EQ(next->getId(), 0);
    EXPECT_EQ(m.getCurrentItem(), nullptr);
    EXPECT_EQ(m.getQueueSize(), 1);

    Item* next2 = m.popNextIfReady(5);

    ASSERT_NE(next2, nullptr);
    EXPECT_EQ(next2->getId(), 1);
    EXPECT_EQ(m.getQueueSize(), 0);
}

TEST(MachineTest, PopAfterFinish) {
    Machine m(0, {5});

    Item item0(0, 0);
    Item item1(1, 0);

    m.pushToQueue(&item0);
    m.pushToQueue(&item1);

    Item* first = m.popNextIfReady(0);

    ASSERT_NE(first, nullptr);

    m.startProcessing(first, 0);

    EXPECT_EQ(m.popNextIfReady(4), nullptr);

    Item* second = m.popNextIfReady(5);

    ASSERT_NE(second, nullptr);
    EXPECT_EQ(second->getId(), 1);
    EXPECT_EQ(m.getQueueSize(), 0);
}

TEST(MachineTest, ZeroProcessingTime) {
    Machine m(0, {0});

    Item item(0, 0);

    m.startProcessing(&item, 7);

    EXPECT_EQ(m.getBusyUntil(), 7);
    EXPECT_TRUE(m.isIdle(7));

    Item item2(1, 0);

    m.pushToQueue(&item2);

    Item* next = m.popNextIfReady(7);

    ASSERT_NE(next, nullptr);
    EXPECT_EQ(next, &item2);
    EXPECT_EQ(m.getCurrentItem(), nullptr);
}

TEST(MachineTest, GetProcessingTime) {
    Machine m(0, {3, 5});

    EXPECT_EQ(m.getProcessingTime(0), 3);
    EXPECT_EQ(m.getProcessingTime(1), 5);
}
