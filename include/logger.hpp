#pragma once

#include <types.hpp>

#include <string>
#include <vector>

namespace conveyor {
    
class Logger {
public:
    enum class Priority {
        STOP   = 0,
        READY  = 1,
        WAIT   = 2,
        START  = 3,
        FINISH = 4
    };

    struct Event {
        Time        time;
        Priority    priority;
        std::string message;

        bool operator>(const Event& other) const {
            if (time != other.time) {
                return time < other.time;
            }

            return static_cast<uint8_t>(priority) > static_cast<uint8_t>(other.priority);
        }
    };

    void logFinish(Time t, ItemId k, ItemType i, MachineId j);
    void logStart(Time t, ItemId k, ItemType i, MachineId j);
    void logWait(Time t, ItemId k, ItemType i, MachineId j, size_t queue_size);
    void logReady(Time t, ItemId k, MachineId j);
    void logStop(Time t);

    void flushCurrentTime(Time current_time);
    void flushAll();

private:
    std::vector<Event> buffer_;
    Time               last_flushed_time_ = 0;

    void pushEvent(Time t, Priority p, std::string msg);
};

} // namespace conveyor
