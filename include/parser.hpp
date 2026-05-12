#pragma once

#include "item.hpp"
#include "machine.hpp"

#include <string>
#include <vector>
#include <memory>

namespace conveyor {

struct ParsedData {
    uint32_t M;
    uint32_t N;

    std::vector<std::vector<Time>>     op_times;
    std::vector<std::vector<ItemType>> initial_queues;
};

class Parser {
public:
    static ParsedData parseFile(const std::string& filename);

    static std::vector<std::unique_ptr<Item>> createItems(const ParsedData& data);

private:
    static std::string readLineSafe(std::istream& is, std::string& original_line);

    static std::vector<int> parseNumbers(const std::string& line,
                                         size_t expected_count,
                                         int min_val,
                                         int max_val,
                                         const std::string& context);
};

} // namespace conveyor
