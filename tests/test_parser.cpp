#include "parser.hpp"

#include <fstream>
#include <sstream>
#include <cstdio>

#include <gtest/gtest.h>

using namespace conveyor;

std::string createTempFile(const std::string& content) {
    std::string filename = "temp_test_input.txt";

    std::ofstream out(filename);
    out << content;
    out.close();

    return filename;
}

TEST(ParserTest, ValidExampleInput) {
    std::string input =
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n";

    std::string fname = createTempFile(input);
    auto data         = Parser::parseFile(fname);

    EXPECT_EQ(data.M, 3);
    EXPECT_EQ(data.N, 2);
    ASSERT_EQ(data.op_times.size(), 2);
    EXPECT_EQ(data.op_times[0][0], 3);
    EXPECT_EQ(data.op_times[0][1], 5);
    EXPECT_EQ(data.op_times[1][0], 4);
    EXPECT_EQ(data.op_times[1][1], 6);
    ASSERT_EQ(data.initial_queues.size(), 2);
    ASSERT_EQ(data.initial_queues[0].size(), 3);
    EXPECT_EQ(data.initial_queues[0][0], 0);
    EXPECT_EQ(data.initial_queues[0][1], 1);
    EXPECT_EQ(data.initial_queues[0][2], 0);
    ASSERT_EQ(data.initial_queues[1].size(), 2);
    EXPECT_EQ(data.initial_queues[1][0], 1);
    EXPECT_EQ(data.initial_queues[1][1], 0);

    std::remove(fname.c_str());
}

TEST(ParserTest, MinimalM2N1) {
    std::string input =
        "2 1\n"
        "7\n"
        "1 0\n";

    std::string fname = createTempFile(input);
    auto data         = Parser::parseFile(fname);

    EXPECT_EQ(data.M, 2);
    EXPECT_EQ(data.N, 1);
    ASSERT_EQ(data.op_times.size(), 1);
    EXPECT_EQ(data.op_times[0][0], 7);
    ASSERT_EQ(data.initial_queues[0].size(), 1);
    EXPECT_EQ(data.initial_queues[0][0], 0);

    std::remove(fname.c_str());
}

TEST(ParserTest, EmptyQueue) {
    std::string input =
        "3 1\n"
        "2\n"
        "3\n"
        "0\n";

    std::string fname = createTempFile(input);
    auto data         = Parser::parseFile(fname);

    EXPECT_EQ(data.initial_queues[0].size(), 0);
    std::remove(fname.c_str());
}

TEST(ParserDeathTest, WrongNumberOfValuesInFirstLine) {
    std::string input = "3\n";
    std::string fname = createTempFile(input);

    EXPECT_EXIT(Parser::parseFile(fname), ::testing::ExitedWithCode(1), "3");

    std::remove(fname.c_str());
}

TEST(ParserDeathTest, ValueOutOfRange) {
    std::string input =
        "3 2\n"
        "10001 5\n"
        "4 6\n"
        "0\n"
        "0\n";

    std::string fname = createTempFile(input);

    EXPECT_EXIT(Parser::parseFile(fname), ::testing::ExitedWithCode(1), "10001");

    std::remove(fname.c_str());
}

TEST(ParserDeathTest, LetterInsteadOfNumber) {
    std::string input =
        "3 2\n"
        "a 5\n"
        "4 6\n"
        "0\n"
        "0\n";

    std::string fname = createTempFile(input);

    EXPECT_EXIT(Parser::parseFile(fname), ::testing::ExitedWithCode(1), "a");

    std::remove(fname.c_str());
}

TEST(ParserDeathTest, InvalidItemTypeInQueue) {
    std::string input =
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "1 2\n"
        "0\n";

    std::string fname = createTempFile(input);

    EXPECT_EXIT(Parser::parseFile(fname), ::testing::ExitedWithCode(1), "2");

    std::remove(fname.c_str());
}

TEST(ParserDeathTest, NegativeQueueCount) {
    std::string input =
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "-1\n"
        "0\n";

    std::string fname = createTempFile(input);

    EXPECT_EXIT(Parser::parseFile(fname), ::testing::ExitedWithCode(1), "-1");

    std::remove(fname.c_str());
}

TEST(ParserTest, ItemNumbering) {
    std::string input =
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "2 0 1\n"
        "1 0\n";

    std::string fname = createTempFile(input);
    auto data         = Parser::parseFile(fname);
    auto items        = Parser::createItems(data);

    ASSERT_EQ(items.size(), 3);
    EXPECT_EQ(items[0]->getId(), 0);
    EXPECT_EQ(items[0]->getCurrentType(), 0);
    EXPECT_EQ(items[1]->getId(), 1);
    EXPECT_EQ(items[1]->getCurrentType(), 1);
    EXPECT_EQ(items[2]->getId(), 2);
    EXPECT_EQ(items[2]->getCurrentType(), 0);

    std::remove(fname.c_str());
}
