#include <iostream>
#include "Thread.hpp"
#include <signal.h>
#include "Fault.h"

#include "ClassA.hpp"
#include "ClassB.hpp"

using namespace    std;

// Worker thread instances
// static WorkerThread workerThread1("WorkerThread1");
// static WorkerThread workerThread2("WorkerThread2");

static A threadA("Workerthread A");
static B threadB("Workerthread B");

static void exitProgram(int32_t interruptSignal)
{
  std::cout <<"---------- Exiting Application ----------" << std::endl;
  std::cout <<"---------- Exiting Application ----------" << std::endl;

  // workerThread1.ExitThread();
  // workerThread2.ExitThread();
  threadA.ExitThread();
  threadB.ExitThread();

  exit(interruptSignal);
}

int main(void)
{    
    signal(SIGINT, exitProgram);
    signal(SIGTERM, exitProgram);     

    std::cout <<"---------- Starting Application ----------" << std::endl;
    std::cout <<"---------- Starting Application ----------" << std::endl;

    // Create worker threads
    // workerThread1.CreateThread();
    // workerThread2.CreateThread();

    threadA.CreateThread();
    threadB.CreateThread();
    // Thread2 should process the can bus read and call the notifyObserver() method which 
    // Forwards the values read from the CANBus to the WorkerThread2 --> Responsible for 
    // the MQTT connectivity

    // Events EventsA = new Events{ ID,}
    //1. WorkerThread1.RegisterObserver(&workerThread2.);
    //2. WorkerThread1.Events(&EventsA)
    //3. WorkerThread1.EventHandlers(&EventhandlersA)

    // Create message to send to worker thread 1
    UserDataA* userData1 = new UserDataA();
    userData1->msg = "Hello world";
    userData1->year = 2017;
    userData1->evId = ClassAEventIds::TASKA_MESSAGE_PRINT_LOG; //TASKA_MESSAGE_MAIN_LOOP; //

    DataMsg<UserDataA> *myData = new DataMsg<UserDataA>(userData1);

    threadA.PostMsg(myData);

    // Give time for messages processing on worker threads
    this_thread::sleep_for(1s);



    UserDataB* userData2 = new UserDataB();
    userData2->msg = "Hello world";
    userData2->year = 2017;
    userData2->evId = ClassBEventIds::TASKB_MESSAGE_PRINT_LOG; //TASKA_MESSAGE_MAIN_LOOP; //

    DataMsg<UserDataB> *myData2 = new DataMsg<UserDataB>(userData2);

    threadB.PostMsg(myData2);

    // Give time for messages processing on worker threads
    this_thread::sleep_for(1s);
    
    while(true); 

    // This should be unreachable
    ASSERT();   

    return 0;
}