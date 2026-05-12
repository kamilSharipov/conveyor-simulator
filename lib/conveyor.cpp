#include "conveyor.hpp"

namespace conveyor {

Conveyor::Conveyor(uint32_t M,
                   uint32_t N,
                   std::vector<std::vector<Time>> op_times) 
    : M_(M), N_(N)
{
    machines_.reserve(N);

    for (MachineId i = 0; i < N; ++ i) {
        machines_.emplace_back(i, op_times.size() > 0 ? op_times[0] : std::vector<Time>{});

        std::vector<Time> col;
        col.reserve(op_times.size());

        for (const auto& row: op_times) {
            col.push_back(row[i]);
        }

        machines_.back() = Machine(i, std::move(col));
    }
}

void Conveyor::loadInitialState(std::vector<std::unique_ptr<Item>> items,
                                const std::vector<std::vector<ItemType>>& initial_queues) {
    items_ = std::move(items);
    total_items = items_.size();

    size_t next_idx = 0;
    for (MachineId i = 0; i < N_; ++i) {
        for (size_t j = 0; j < initial_queues[i].size(); ++j) {
            machines_[i].pushToQueue(items_[next_idx].get());
            ++next_idx;
        }
    }
}

void Conveyor::handleFinish(MachineId machine_id,
                            Item* item,
                            ItemType finished_op,
                            Time finish_time,
                            Logger& logger) {
    logger.logFinish(finish_time, item->getId(), finished_op, machine_id);

    if (finished_op == M_ - 2) {
        item->setState(Item::State::COMPLETED);
        logger.logReady(finish_time, item->getId(), machine_id);
        ++completed_count;

        return;
    }

    item->advanceType();

    ItemType  next_type = item->getCurrentType();
    MachineId target    = selectBestMachine();

    if (machines_[target].isIdle(finish_time)) {
        machines_[target].startProcessing(item, finish_time);
        logger.logStart(finish_time, item->getId(), next_type, target);

        Time duration = machines_[target].getProcessingTime(next_type);

        event_queue_.push({finish_time + duration, target, item->getId(), next_type});
    } else {
        size_t queue_size = machines_[target].getQueueSize();

        machines_[target].pushToQueue(item);
        logger.logWait(finish_time, item->getId(), next_type, target, queue_size);
    }
}

MachineId Conveyor::selectBestMachine() const {
    MachineId best = 0;
    Time best_wait = machines_[0].calculateWaitTime();

    for (MachineId j = 1; j < N_; ++j) {
        Time wait = machines_[j].calculateWaitTime();
        if (wait < best_wait) {
            best_wait = wait;
            best = j;
        }
    }

    return best;
}

void Conveyor::tryStartNext(MachineId machine_id, Time current_time, Logger& logger) {
    if (auto* item = machines_[machine_id].popNextIfReady(current_time)) {
        ItemType type = item->getCurrentType();

        machines_[machine_id].startProcessing(item, current_time);
        logger.logStart(current_time, item->getId(), type, machine_id);

        Time duration = machines_[machine_id].getProcessingTime(type);
        event_queue_.push({current_time + duration, machine_id, item->getId(), type});
    }
}

Time Conveyor::run(Logger& logger) {
    for (MachineId i = 0; i < N_; ++i) {
        tryStartNext(i, 0, logger);
    }

    while (!event_queue_.empty() && event_queue_.top().time == 0) {
        auto event = event_queue_.top();
        event_queue_.pop();
        handleFinish(event.machine_id,
                     items_[event.item_id].get(),
                     event.operation_type,
                     event.time,
                     logger);
        tryStartNext(event.machine_id, event.time, logger);
    }

    logger.flushCurrentTime(0);

    Time current_time = 0;
    while (completed_count < total_items && !event_queue_.empty()) {
        current_time = event_queue_.top().time;

        while (!event_queue_.empty() && event_queue_.top().time == current_time) {
            auto event = event_queue_.top();
            event_queue_.pop();

            handleFinish(event.machine_id,
                         items_[event.item_id].get(),
                         event.operation_type,
                         current_time,
                         logger);
            tryStartNext(event.machine_id, current_time, logger);
        }

        logger.flushCurrentTime(current_time);
    }

    logger.logStop(current_time);
    logger.flushAll();
    return current_time;
}

} // namespace conveyor