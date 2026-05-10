#include "logger.hpp"

#include <algorithm>
#include <iostream>

namespace conveyor {
    
void Logger::pushEvent(Time t, Priority p, std::string msg) {
    buffer_.push_back({t, p, std::move(msg)});
}

void Logger::logFinish(Time t, ItemId k, ItemType i, MachineId j) {
    pushEvent(t, Priority::FINISH, "finish " + 
                                   std::to_string(t) + " " +
                                   std::to_string(k) + " " +
                                   std::to_string(i) + " " +
                                   std::to_string(j));
}

void Logger::logStart(Time t, ItemId k, ItemType i, MachineId j) {
    pushEvent(t, Priority::START, "start " + 
                                   std::to_string(t) + " " +
                                   std::to_string(k) + " " +
                                   std::to_string(i) + " " +
                                   std::to_string(j));
}

void Logger::logWait(Time t, ItemId k, ItemType i, MachineId j, size_t queue_size) {
    pushEvent(t, Priority::WAIT, "wait " + 
                                 std::to_string(t) + " " +
                                 std::to_string(k) + " " +
                                 std::to_string(i) + " " +
                                 std::to_string(j) + " " +
                                 std::to_string(queue_size));
}

void Logger::logReady(Time t, ItemId k, MachineId j) {
    pushEvent(t, Priority::READY, "ready " +
                                  std::to_string(t) + " " +
                                  std::to_string(k) + " " +
                                  std::to_string(j));
}

void Logger::logStop(Time t) {
    pushEvent(t, Priority::STOP, "stop " + std::to_string(t));
}

void Logger::flushCurrentTime(Time current_time) {
    std::sort(buffer_.begin(), buffer_.end(), 
              [](const Event& a, const Event& b) { return a > b; });

    for (auto it = buffer_.begin(); it != buffer_.end(); ) {
        if (it->time == current_time) {
            std::cout << it->message << '\n';
            it = buffer_.erase(it);
        } else {
            ++it;
        }
    }

    last_flushed_time_ = current_time;
}

void Logger::flushAll() {
    std::sort(buffer_.begin(), buffer_.end(), 
              [](const Event& a, const Event& b) { return a > b; });

    for (const auto& event : buffer_) {
        std::cout << event.message << '\n';
    }

    buffer_.clear();
}

} // namespace conveyor
