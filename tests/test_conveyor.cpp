#include "conveyor.hpp"
#include "parser.hpp"

#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

using namespace conveyor;

std::string runSimulation(const std::string& input) {
    std::string fname = "temp_sim_input.txt";

    std::ofstream out(fname);
    out << input;
    out.close();

    auto data  = Parser::parseFile(fname);
    auto items = Parser::createItems(data);

    Conveyor conv(data.M, data.N, data.op_times);
    conv.loadInitialState(std::move(items), data.initial_queues);

    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());

    Logger logger;
    conv.run(logger);
    std::cout.rdbuf(old_cout);

    std::remove(fname.c_str());

    return buffer.str();
}

TEST(ConveyorTest, ExampleFromTechSpecFixedT) {
    std::string input =
        "3 2\n"
        "3 5\n"
        "4 4\n"
        "3 0 1 0\n"
        "2 1 0\n";

    std::string output = runSimulation(input);

    EXPECT_NE(output.find("start 0 0 0 0"), std::string::npos);
    EXPECT_NE(output.find("start 0 3 1 1"), std::string::npos);
    EXPECT_NE(output.find("finish 3 0 0 0"), std::string::npos);
    EXPECT_NE(output.find("start 3 1 1 0"), std::string::npos);
    EXPECT_NE(output.find("wait 3 0 1 1 1"), std::string::npos);
    EXPECT_NE(output.find("ready"), std::string::npos);
    EXPECT_NE(output.find("stop"), std::string::npos);

    size_t stop_pos = output.rfind("stop");

    EXPECT_GT(stop_pos, output.rfind("ready"));
}

TEST(ConveyorTest, OriginalExampleT6) {
    std::string input =
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n";

    std::string output = runSimulation(input);

    EXPECT_NE(output.find("finish 6 3 1 1"), std::string::npos);
    EXPECT_NE(output.find("finish 17 0 1 1"), std::string::npos);
    EXPECT_NE(output.find("stop 18"), std::string::npos);
}

TEST(ConveyorTest, ImmediateStartAfterFinish) {
    std::string input =
        "3 1\n"
        "2\n"
        "3\n"
        "2 0 1\n";

    std::string output = runSimulation(input);

    size_t pos_finish = output.find("finish 2 0 0 0");
    size_t pos_start  = output.find("start 2 1 1 0");

    EXPECT_LT(pos_finish, pos_start);
}

TEST(ConveyorTest, BestMachineSelectionByWaitTime) {
    std::string input =
        "3 2\n"
        "2 10\n"
        "1 1\n"
        "2 0 0\n"
        "0\n";

    std::string output = runSimulation(input);

    EXPECT_NE(output.find("start 2 0 1 1"), std::string::npos);
}

TEST(ConveyorTest, ZeroOperationTime) {
    std::string input =
        "3 1\n"
        "0\n"
        "1\n"
        "1 0\n";

    std::string output = runSimulation(input);

    EXPECT_NE(output.find("finish 0 0 0 0"), std::string::npos);
    EXPECT_NE(output.find("start 0 0 1 0"), std::string::npos);
    EXPECT_NE(output.find("finish 1 0 1 0"), std::string::npos);
    EXPECT_NE(output.find("ready 1 0 0"), std::string::npos);
    EXPECT_NE(output.find("stop 1"), std::string::npos);
}

TEST(ConveyorTest, StopTimeCorrect) {
    std::string input =
        "2 1\n"
        "5\n"
        "1 0\n";

    std::string output = runSimulation(input);

    EXPECT_NE(output.find("stop 5"), std::string::npos);
}

TEST(ConveyorTest, StressSmall) {
    std::ostringstream inp;
    inp << "5 5\n";

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) inp << (rand() % 10 + 1) << " ";
        inp << "\n";
    }

    inp << "2 0 1\n";
    inp << "2 2 3\n";
    inp << "1 0\n";
    inp << "0\n";
    inp << "1 1\n";

    std::string output = runSimulation(inp.str());

    EXPECT_NE(output.find("stop"), std::string::npos);
}
