#pragma once

#include "item.hpp"

#include <deque>
#include <vector>

namespace conveyor {

class Machine {
public:
    explicit Machine(MachineId id, std::vector<Time> op_times);

    MachineId   getId() const;

    void        pushToQueue(Item* item);
    size_t      getQueueSize() const;
    Time        calculateWaitTime() const;

    bool        isIdle(Time t) const;
    void        startProcessing(Item* item, Time start_time);
    Time        getBusyUntil() const;
    const Item* getCurrentItem() const;
    Time        getProcessingTime(ItemType type) const;

    Item*       popNextIfReady(Time t);

private:
    MachineId          id_;
    std::vector<Time>  op_times_;
    std::deque<Item*>  queue_;
    Item*              current_item_;
    Time               busy_until_;
};

} // namespace conveyor
