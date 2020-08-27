#ifndef _CLASSB_H
#define _CLASSB_H

//#include "AnyCallable.hpp"
#include "Thread.hpp"
#include <cmath>
#include <iostream>
#include <functional>
#include <map>
#include "Fault.h"

#include "Observer.hpp" // This unit is client, it's function will be called at notification.

//struct ThreadMsg;
using namespace std;
using namespace std::placeholders; 

// Must use name space otherwise there will be a conflict in global variables
typedef map<enum ClassBEventIds , std::function<void(int)>> EventMapB;

enum class ClassBEventIds 
{ 
  TASKB_MESSAGE_MIN               = INT16_MIN, 
  TASKB_MESSAGE_TO_MQTT           = 0,
  TASKB_MESSAGE_PRINT_LOG         = 1, 
  TASKB_MESSAGE_MAIN_ON_B         = 2, 
  TASKB_MESSAGE_SEND_TO_MQTT      = 3,
  TASKB_MESSAGE_MAX               = 4, 
 };


#pragma pack(push, 1) 
struct UserDataB
{
	std::string msg;
	int year;
  ClassBEventIds evId;
};
#pragma pack(pop)

class B : public WorkerThread, public Observer
{

private:

  vector<int> mData;// This is the data structure of entry event to the queue
  // typedef map<enum class EventIds, AnyCallable<void>> EventMapB;
  //typedef map<int, AnyCallable<void>> EventMapB;
  
  static EventMapB sEventHandlersList;  // This contains the list of handler functions which can be 
  // initialized at object init section.

public:

  B(const char* threadName) : WorkerThread::WorkerThread(threadName) 
  {
    Init();
  }

  void Init() // Make a list of event handlers...for every new event and associated handlers add here.
  { 
    sEventHandlersList.insert(
      { ClassBEventIds::TASKB_MESSAGE_PRINT_LOG, std::bind(&B::HandlePrintLogMessage, this, std::placeholders::_1)});
    sEventHandlersList.insert(
      { ClassBEventIds::TASKB_MESSAGE_SEND_TO_MQTT, std::bind(&B::HandleReadCANBusMessage, this)});

  }

  void MainLoop(int x)
  {
    this_thread::sleep_for(50ms);
    std::cout << "Handle MainLoop" << std::endl;        
  }

  void HandlePrintLogMessage(int x)
  {
    this_thread::sleep_for(50ms);
    std::cout << "Handle print log message for thread B!" << std::endl;
  }

  void HandleReadCANBusMessage()
  {
    this_thread::sleep_for(50ms);
    std::cout << "I'm sending data to MQTT: THREAD B!" << std::endl;
  }

  virtual void ProcessUserEvent(const ThreadMsg* incoming) override
  { 
    ASSERT_TRUE(incoming->msg != NULL);

    auto userData = static_cast<const Msg*>(incoming->msg);  

    const DataMsg<UserDataB> *test = static_cast<const DataMsg<UserDataB>*>(userData);

    // std::cout << test->getPayload().msg.c_str() << std::endl
    //           << static_cast<int>(test->getPayload().evId) << std::endl
    //           << "on :" << this->getThreadName() << std::endl;

    ASSERT_TRUE((test->getPayload().evId >= ClassBEventIds::TASKB_MESSAGE_MIN) && (test->getPayload().evId < ClassBEventIds::TASKB_MESSAGE_MAX));

    sEventHandlersList[test->getPayload().evId](21);
    delete userData; 
   
    // Implement Notify() to inform other threads...
    // For this we need a list of observers.
  }

  virtual void ProcessTimerEvent(const ThreadMsg* incoming) override
  { 
    // OK I will send the user event to read the CAN bus  

    UserDataB* userData1 = new UserDataB();
    userData1->msg = "SEND TO MQTT";
    userData1->evId = ClassBEventIds::TASKB_MESSAGE_SEND_TO_MQTT; 

    DataMsg<UserDataB> *myData = new DataMsg<UserDataB>(userData1);

    this->PostMsg(myData);
    // Implement Notify() to inform other threads...
    // For this we need a list of observers.
  }

  virtual void update(ObseverMsg* data) override{    

    // Cast data back to the struct of type A
    ObseverDataMsg<float> *test = static_cast<ObseverDataMsg<float>*>(data);
    //std::cout << test->getPayload().msg.c_str() << std::endl

    std::cout << this->getThreadName() 
              << ":"
              << "Data is uplopading to the cloud: notified from WorkerThread A!" 
              << std::endl
              << "Data val: " 
              << test->getPayload() 
              << std::endl
              << std::endl;

    delete data;
  }

};

EventMapB B::sEventHandlersList; 

#endif 