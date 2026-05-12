#include "machine.hpp"

#include <stdexcept>

namespace conveyor {

Machine::Machine(MachineId id, std::vector<Time> op_times) 
    : id_{id}
    , op_times_{std::move(op_times)}
    , current_item_{nullptr}
    , busy_until_{0} 
{
    if (op_times_.empty()) {
        throw std::invalid_argument{
            "Machine " + std::to_string(id_) + ": op_times cannot be empty"
        };
    }
}

MachineId Machine::getId() const {
    return id_;
}

void Machine::pushToQueue(Item* item) {
    if (item == nullptr) {
        throw std::invalid_argument{
            "Machine " + std::to_string(id_) + ": pushToQueue called with nullptr"
        };
    }

    queue_.push_back(item);
}

size_t Machine::getQueueSize() const {
    return queue_.size();
}

Time Machine::calculateWaitTime() const {
    if (op_times_.empty()) {
        return 0;
    }

    Time sum = 0;

    for (const auto& item : queue_) {
        sum += op_times_[item->getCurrentType()];
    }

    return sum;
}

bool Machine::isIdle(Time t) const {
    return current_item_ == nullptr || busy_until_ <= t;
}

void Machine::startProcessing(Item* item, Time start_time) {
    if (item == nullptr) {
        throw std::invalid_argument{
            "Machine " + std::to_string(id_) + ": startProcessing called with nullptr"
        };
    }

    Time duration = op_times_.empty() ? 0 : op_times_[item->getCurrentType()];
    current_item_ = item;
    busy_until_   = start_time + duration;
}

Time Machine::getBusyUntil() const {
    return busy_until_;
}

const Item* Machine::getCurrentItem() const {
    return current_item_;
}

Time Machine::getProcessingTime(ItemType type) const {
    if (op_times_.empty()) {
        return 0;
    }

    return op_times_[type];
}

Item* Machine::popNextIfReady(Time t) {
    if (current_item_ != nullptr && busy_until_ <= t) {
        current_item_ = nullptr;
    }

    if (current_item_ == nullptr && !queue_.empty()) {
        Item* next = queue_.front();
        queue_.pop_front();

        return next;
    }

    return nullptr;
}

} // namespace conveyor
