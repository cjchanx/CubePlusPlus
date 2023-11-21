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
#include "Mutex.hpp"

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

    void HandleConsistencyError();

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
    Mutex mtx_;
};

template<typename T, const size_t SIZE>
PQueue<T, SIZE>::PQueue() : 
    rtQueue(SIZE)
 {}

template<typename T, const size_t SIZE>
bool PQueue<T, SIZE>::Send(const T& item, uint8_t priority) {
    if (etlQueue.full()) {
        return false;
    }
    etlQueue.push({item, priority});
    return ;
}

template<typename T, const size_t SIZE>
bool PQueue<T, SIZE>::Receive(T& item, uint32_t timeout_ms) {
    // RTOS Queue Poll
    if(rtQueue.Receive(timeout_ms)) {
        return false;
    }

    // If there is an empty queue, we have a queue consistency issue
    if (etlQueue.empty()) {
        return false;
    }
    item = etlQueue.top().data_;
    etlQueue.pop();
    return xQueueReceive(rtQueueHandle, &item, 0) == pdTRUE;

    // If we failed to acquire the priority queue mutex, you must add another item to the rtos queue to ensure size consistency
	if(!mtx_->Lock(DEFAULT_PQUEUE_MTX_TIMEOUT_MS)) {
		NotifySelf();
		return false;
	}

	// We have access to the priority queue
	bool res = priorityQueue_->Pop(data);

	// -- below should be wrapped in a function called HandleError(); --
	// If we managed to get here, and failed to pop, there's a data consistency error - which should not be possible except in the case of a bug
	if(!res) {
		SYS_ASSERT(++pqErrorCount > DEFAULT_PQUEUE_ERROR_COUNT_MAX, 
			"PQueue data consistency faults exceeded limits");
	
		SYS_PRINT("WARNING: PQueue Data Consistency Error\r\n");
	}
}

template<typename T, const size_t SIZE>
bool PQueue::ReceiveWait(T& item) {
    //TODO: Double check whether this HAL_MAX_DELAY works across various tickrates (or if we need to do a tick conversion...)
    return Receive(item, HAL_MAX_DELAY); 
}

template<typename T, const size_t SIZE>
bool PQueue::HandleConsistencyError() {
    //TODO:
    // Warn (PRINT) and match the RT Queue size to the PQueue size
}

#endif // CUBE_PLUSPLUS_INCLUDE_PRIORITY_QUEUE_H