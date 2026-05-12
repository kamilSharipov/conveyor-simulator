#include "logger.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace conveyor;

class LoggerTest : public ::testing::Test {
protected:
    Logger            logger;
    std::stringstream buffer;
    std::streambuf*   old_cout;

    void SetUp() override {
        old_cout = std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(old_cout);
    }

    std::string getOutput() {
        return buffer.str();
    }
};

TEST_F(LoggerTest, PriorityOrderSameTime) {
    logger.logReady(5, 0, 0);
    logger.logWait(5, 1, 1, 1, 0);
    logger.logStart(5, 2, 0, 2);
    logger.logFinish(5, 3, 0, 3);
    logger.flushCurrentTime(5);

    std::string out = getOutput();

    size_t pos_finish = out.find("finish");
    size_t pos_start  = out.find("start");
    size_t pos_wait   = out.find("wait");
    size_t pos_ready  = out.find("ready");

    EXPECT_LT(pos_finish, pos_start);
    EXPECT_LT(pos_start, pos_wait);
    EXPECT_LT(pos_wait, pos_ready);
}

TEST_F(LoggerTest, StopIsLast) {
    logger.logReady(10, 0, 0);
    logger.logStop(10);
    logger.flushAll();

    std::string out = getOutput();
    size_t stop_pos = out.rfind("stop");

    ASSERT_NE(stop_pos, std::string::npos);

    size_t end_of_stop = stop_pos + 7;

    EXPECT_EQ(out.substr(end_of_stop), "\n");
}

TEST_F(LoggerTest, FlushCurrentTimeDoesNotOutputLaterEvents) {
    logger.logStart(3, 0, 0, 0);
    logger.logFinish(5, 1, 0, 1);
    logger.flushCurrentTime(3);

    std::string out = getOutput();

    EXPECT_NE(out.find("start 3"), std::string::npos);
    EXPECT_EQ(out.find("finish 5"), std::string::npos);
}
