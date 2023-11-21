/**
 ******************************************************************************
 * File Name          : PQueue.hpp
 * Description        :
 *
 *    PQueue is a wrapper for RTOS xQueues and ETL Priority Queues to ensure
 *    proper signaling and functionality within FreeRTOS
 ******************************************************************************
*/
#ifndef CUBE_PLUSPLUS_INCLUDE_PRIORITY_QUEUE_H
#define CUBE_PLUSPLUS_INCLUDE_PRIORITY_QUEUE_H

/* Includes ------------------------------------------------------------------*/
#include "etl/priority_queue.h"
#include "TQueue.hpp"

/* Constants and Definitions -------------------------------------------------*/
enum Priority : uint8_t {
    HIGH = 200,   // 200 so +- ~50 for fine adjustment
    NORMAL = 127, // 127 centered
    LOW = 50,     // 50 so +- ~50 for fine adjustment

    // Alias enumerations
    MID = NORMAL,
};

/* Class ---------------------------------------------------------------------*/
template<typename T, const size_t SIZE>
class PQueue {
public:
    PQueue();

    bool Send(const T& item, uint8_t priority = Priority::NORMAL);
    bool Receive(T& item, uint32_t timeout_ms = 0);
    bool ReceiveWait(T& item);

    bool IsEmpty() const { return etlQueue.full(); }
    bool IsFull() const { return etlQueue.empty(); }
    uint16_t GetCurrentCount() const { return etlQueue.size(); }
    uint16_t GetMaxDepth() const { return etlQueue.max_size(); }

private:
    struct PriorityQueueItem {
        T data_;
        uint8_t priority_;

        bool operator<(const PriorityQueueItem& other) const {
            return priority_ < other.priority_;
        }
    };

    TQueue<uint8_t> rtQueue;
    etl::priority_queue<PriorityQueueItem, SIZE> etlQueue;
};

template<typename T, const size_t SIZE>
PQueue<T, SIZE>::PQueue() : rtQueueHandle(xQueueCreate(SIZE, sizeof(T))) {}

template<typename T, const size_t SIZE>
bool PQueue<T, SIZE>::Send(const T& item, uint8_t priority) {
    if (etlQueue.full()) {
        return false;
    }
    etlQueue.push({item, priority});
    return rtQueue.;
}

template<typename T, const size_t SIZE>
bool PQueue<T, SIZE>::Receive(T& item, uint32_t timeout_ms) {
    // RTOS Queue Poll
    if(rt)


    // ETL Queue Operations
    if (etlQueue.empty()) {
        return false;
    }
    item = etlQueue.top().data_;
    etlQueue.pop();
    return xQueueReceive(rtQueueHandle, &item, 0) == pdTRUE;
}


template<typename T, const size_t SIZE>
bool PQueue::ReceiveWait(T& item) {
    return Receive(item, HAL_MAX_DELAY);
}

#endif // CUBE_PLUSPLUS_INCLUDE_PRIORITY_QUEUE_H