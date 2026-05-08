#include "item.hpp"

namespace conveyor {

Item::Item(ItemId id, ItemType initial_type)
    : id_(id), current_type_(initial_type), state_(State::QUEUED) {}

ItemId Item::getId() const {
    return id_;
}

ItemType Item::getCurrentType() const {
    return current_type_;
}

Item::State Item::getState() const {
    return state_;
}

void Item::setState(State new_state) {
    state_ = new_state;
}

void Item::advanceType() {
    ++current_type_;
}

bool Item::isCompleted() const {
    return state_ == State::COMPLETED;
}

} // namespace conveyor