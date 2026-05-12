#include "parser.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <iostream>

namespace conveyor {

std::string Parser::readLineSafe(std::istream& is, std::string& original_line) {
    if (!std::getline(is, original_line)) {
        throw std::runtime_error{"Unexpected end of file"};
    }

    std::string        cleaned;
    std::istringstream iss(original_line);
    std::string        token;

    while (iss >> token) {
        cleaned += token + " ";
    }

    return cleaned;
}

std::vector<int> Parser::parseNumbers(const std::string& line,
                                      size_t expected_count,
                                      int min_val,
                                      int max_val,
                                      const std::string& context) {
    std::vector<int>   result;
    std::istringstream iss(line);
    std::string        token;

    while (iss >> token) {
        try {
            size_t pos;
            long long val = std::stoll(token, &pos);

            if (pos != token.size()) {
                throw std::invalid_argument{"Invalid number format"};
            }

            if (val < min_val || val > max_val) {
                throw std::out_of_range{"Value out of range"};
            }

            result.push_back(static_cast<int>(val));
        } catch (const std::exception&) {
            throw std::runtime_error{context + ": cannot parse '" + token + "'"};
        }
    }

    if (result.size() != expected_count) {
        throw std::runtime_error{
            context + ": expected " + std::to_string(expected_count) + 
            " numbers, got " + std::to_string(result.size())
        };
    }

    return result;
}

ParsedData Parser::parseFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        std::exit(1);
    }

    ParsedData  data;
    std::string line, original;

    try {
        line    = readLineSafe(file, original);
        auto mn = parseNumbers(line, 2, 1, 100, "First line (M N)");

        data.M = static_cast<uint32_t>(mn[0]);
        data.N = static_cast<uint32_t>(mn[1]);

        data.op_times.resize(std::max(1u, data.M - 1), std::vector<Time>(data.N));
        for (uint32_t i = 0; i < data.M - 1; ++i) {
            line       = readLineSafe(file, original);
            auto times = parseNumbers(line, data.N, 0, 10000, "Operation times row " + std::to_string(i));

            for (uint32_t j = 0; j < data.N; ++j) {
                data.op_times[i][j] = static_cast<Time>(times[j]);
            }
        }

        data.initial_queues.resize(data.N);
        for (uint32_t i = 0; i < data.N; ++i) {
            line = readLineSafe(file, original);

            std::istringstream iss(line);
            int qi;

            if (!(iss >> qi) || qi < 0) {
                throw std::runtime_error{"Queue line " + std::to_string(i) + ": invalid count"};
            }

            data.initial_queues[i].reserve(qi);
            for (int j = 0; j < qi; ++j) {
                int type;

                if (!(iss >> type) || type < 0 || type >= static_cast<int>(data.M - 1)) {
                    throw std::runtime_error{"Queue line " + std::to_string(i) + ": invalid item type at position " + std::to_string(j)};
                }

                data.initial_queues[i].push_back(static_cast<ItemType>(type));
            }
        }
    } catch (...) {
        throw std::runtime_error{original};
    }

    return data;
}

std::vector<std::unique_ptr<Item>> Parser::createItems(const ParsedData& data) {
    std::vector<std::unique_ptr<Item>> items;
    ItemId next_id = 0;

    for (uint32_t i = 0; i < data.N; ++i) {
        for (ItemType type : data.initial_queues[i]) {
            items.push_back(std::make_unique<Item>(next_id++, type));
        }
    }

    return items;
}

} // namespace conveyor
