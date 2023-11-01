/**
 ******************************************************************************
 * File Name          : UARTTask.hpp
 * Description        :
 ******************************************************************************
*/
#ifndef CUBE_PLUSPLUS_CUBETASK_HPP_
#define CUBE_PLUSPLUS_CUBETASK_HPP_

/* Includes ------------------------------------------------------------------*/
#include "Task.hpp"
#include "SystemDefines.hpp"
#include "CubeDefines.hpp"
#include "UARTDriver.hpp"



/* Macros ------------------------------------------------------------------*/
enum CUBE_TASK_COMMANDS {
    CUBE_TASK_COMMAND_NONE = 0,

    CUBE_TASK_COMMAND_SEND_DEBUG,

    CUBE_TASK_COMMAND_MAX
};


/* Class ------------------------------------------------------------------*/
class CubeTask : public Task
{
public:
    static CubeTask& Inst() {
        static CubeTask inst;
        return inst;
    }

    void InitTask();

protected:
    static void RunTask(void* pvParams) { CubeTask::Inst().Run(pvParams); } // Static Task Interface, passes control to the instance Run();

    void Run(void* pvParams);    // Main run code

    void HandleCommand(Command& cm);

private:
    CubeTask() : Task(UART_TASK_QUEUE_DEPTH_OBJS) {}    // Private constructor
    CubeTask(const CubeTask&);                        // Prevent copy-construction
    CubeTask& operator=(const CubeTask&);            // Prevent assignment
};


#endif    // CUBE_PLUSPLUS_CUBETASK_HPP_
