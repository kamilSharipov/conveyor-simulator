#include "parser.hpp"
#include "conveyor.hpp"
#include "logger.hpp"

#include <iostream>
#include <exception>

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    try {
        auto data  = conveyor::Parser::parseFile(argv[1]);
        auto items = conveyor::Parser::createItems(data);

        conveyor::Conveyor conv(data.M, data.N, data.op_times);
        conv.loadInitialState(std::move(items), data.initial_queues);

        conveyor::Logger logger;
        conv.run(logger);

        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';

        return 1;
    }
}
