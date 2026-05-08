#pragma once

#include "types.hpp"

namespace conveyor {

class Item {
public:
    enum class State {
        QUEUED,
        PROCESSING,
        COMPLETED
    };

    explicit Item(ItemId id, ItemType initial_type);

    ItemId getId() const;
    ItemType getCurrentType() const;
    State getState() const;

    void setState(State new_state);
    void advanceType();
    bool isCompleted() const;

private:
    ItemId id_;
    ItemType current_type_;
    State state_;
};

} // namespace conveyor
