
// Standard library includes
#include <iostream>
#include <signal.h>

// Third party includes
#include "mqtt/async_client.h"

// User defined inlcudes
#include "ClassA.hpp"
#include "ClassB.hpp"
#include "Fault.h"
#include "Thread.hpp"

using namespace std;

// Worker thread instances
static A threadA("Workerthread A");
static B threadB("Workerthread B");

static void
exitProgram(int32_t interruptSignal) {
    std::cout << "---------- Exiting Application ----------" << std::endl;
    std::cout << "---------- Exiting Application ----------" << std::endl;

    threadA.ExitThread();
    threadB.ExitThread();

    exit(interruptSignal);
}

int
main(void) {
    signal(SIGINT, exitProgram);
    signal(SIGTERM, exitProgram);

    std::cout << "---------- Starting Application ----------" << std::endl;
    std::cout << "---------- Starting Application ----------" << std::endl;

    // Create worker threads
    threadA.CreateThread();
    threadB.CreateThread();

    threadA.registerObserver(&threadB);  // threadB is the observer of threadA.

    // // Create message to send to worker thread 1
    // UserDataA* userData1 = new UserDataA();
    // userData1->msg = "Hello world";
    // userData1->evId = ClassAEventIds::TASKA_MESSAGE_PRINT_LOG;
    // DataMsg<UserDataA> *myData = new DataMsg<UserDataA>(userData1);
    // threadA.PostMsg(myData);

    // Give time for messages processing on worker threads
    // this_thread::sleep_for(1s);

    // UserDataB* userData2 = new UserDataB();
    // userData2->msg = "Goodbye world";
    // userData2->evId = ClassBEventIds::TASKB_MESSAGE_PRINT_LOG;
    // DataMsg<UserDataB> *myData2 = new DataMsg<UserDataB>(userData2);
    // threadB.PostMsg(myData2);

    // Give time for messages processing on worker threads
    std::this_thread::sleep_for(
        std::chrono::milliseconds(5000));  // Allow some delay before exiting timer

    threadB.ExitTimer();  // By default every thread has one timer created
    // Current we don't need timer for thread B, in future if we need we can
    // activate it again.

    while(true)
        ;

    // This should be unreachable
    ASSERT();

    return 0;
}