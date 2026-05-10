#pragma once

#include "item.hpp"
#include "machine.hpp"
#include "logger.hpp"

#include <queue>
#include <memory>

namespace conveyor {

class Conveyor {
public:
    explicit Conveyor(uint32_t M,
                      uint32_t N,
                      std::vector<std::vector<Time>> op_times);

    void loadInitialItems(std::vector<std::unique_ptr<Item>> items);

    Time run(Logger& logger);

private:
    struct SimulationEvent {
        Time      time;
        MachineId machine_id;
        ItemId    item_id;
        ItemType  operation_type;

        bool operator>(const SimulationEvent& other) {
            return time > other.time;
        }
    };

    uint32_t M_, N_;

    std::vector<Machine>               machines_;
    std::vector<std::unique_ptr<Item>> items_;

    std::priority_queue<SimulationEvent, std::vector<SimulationEvent>,
                        std::greater<>> event_queue_;

    size_t completed_count = 0;
    size_t total_items     = 0;

    MachineId selectBestMachine(ItemType next_type, Time current_time) const;

    void handleFinish(MachineId machine_id,
                      Item* item,
                      ItemType finished_op,
                      Time finish_time,
                      Logger& logger);

    void tryStartNext(MachineId machine_id, Time current_time, Logger& logger);
};

} // namespace conveyor
