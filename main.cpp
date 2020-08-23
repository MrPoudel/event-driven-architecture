#include <iostream>
#include "Thread.hpp"
#include <signal.h>
#include "Fault.h"

using namespace    std;

// Worker thread instances
static WorkerThread workerThread1("WorkerThread1");
static WorkerThread workerThread2("WorkerThread2");

static void exitProgram(int32_t interruptSignal)
{
  std::cout <<"---------- Exiting Application ----------" << std::endl;
  std::cout <<"---------- Exiting Application ----------" << std::endl;

  workerThread1.ExitThread();
  workerThread2.ExitThread();

  exit(interruptSignal);
}

int main(void)
{    
    signal(SIGINT, exitProgram);
    signal(SIGTERM, exitProgram);
    
    // Create worker threads
    workerThread1.CreateThread();
    workerThread2.CreateThread();

    std::cout <<"---------- Exiting Application ----------" << std::endl;
    std::cout <<"---------- Exiting Application ----------" << std::endl;

    // Create message to send to worker thread 1
    UserData* userData1 = new UserData();
    userData1->msg = "Hello world";
    userData1->year = 2017;

    // Post the message to worker thread 1
    workerThread1.PostMsg(userData1);

    // Create message to send to worker thread 2
    UserData* userData2 = new UserData();
    userData2->msg = "Goodbye world";
    userData2->year = 2017;

    // Post the message to worker thread 2
    workerThread2.PostMsg(userData2);

    // Give time for messages processing on worker threads
    this_thread::sleep_for(1s);
    
    while(true); 

    // This should be unreachable
    ASSERT();   

    return 0;
}