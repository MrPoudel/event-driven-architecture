#ifndef _CLASSA_H
#define _CLASSA_H

//#include "AnyCallable.hpp"
#include "Thread.hpp"
#include <cmath>
#include <iostream>
#include <functional>
#include <map>
#include <algorithm>

//struct ThreadMsg;
using namespace std;
using namespace std::placeholders; 

typedef map<enum ClassAEventIds , std::function<void(int)>> EventMap;

enum class ClassAEventIds 
{ 
  TASKA_MESSAGE_MIN               = INT16_MIN, 
  TASKA_MESSAGE_TO_MQTT           = 0,
  TASKA_MESSAGE_PRINT_LOG         = 1, 
  TASKA_MESSAGE_MAIN_LOOP         = 2, 
  TASKA_MESSAGE_READ_CAN_BUS      = 3, 
  TASKA_MESSAGE_MAX               = 4, 
 };


#pragma pack(push, 1) 
struct UserDataA
{
	std::string msg;
  ClassAEventIds evId;
};
#pragma pack(pop)

class A : public WorkerThread
{

private:

  vector<int> mData;// This is the data structure of entry event to the queue
  std::vector<Observer *> observers; // observers
  
  static EventMap sEventHandlersList;  // This contains the list of handler functions which can be 
  // initialized at object init section.

public:

  A(const char* threadName) : WorkerThread::WorkerThread(threadName) 
  {
    Init();
  }

  void Init() // Make a list of event handlers...for every new event and associated handlers add here.
  { 
    sEventHandlersList.insert(
      { ClassAEventIds::TASKA_MESSAGE_PRINT_LOG, std::bind(&A::HandlePrintLogMessage, this, std::placeholders::_1)});
    sEventHandlersList.insert(
      { ClassAEventIds::TASKA_MESSAGE_MAIN_LOOP, std::bind(&A::MainLoop, this, std::placeholders::_1)});
    sEventHandlersList.insert(
      { ClassAEventIds::TASKA_MESSAGE_READ_CAN_BUS, std::bind(&A::HandleReadCANBusMessage, this)});
  }

  void MainLoop(int x)
  {
    this_thread::sleep_for(50ms);
    std::cout << "Handle MainLoop" << std::endl;        
  }

  void HandlePrintLogMessage(int x)
  {
    this_thread::sleep_for(50ms);
    std::cout << "Handle print log message for thread A!" << std::endl;
  }

  void HandleReadCANBusMessage()
  {
    this_thread::sleep_for(50ms);
    std::cout << "I'm reading data from CAN BUS: THREAD A!" << std::endl;
    notifyObservers();
  }

  virtual void ProcessUserEvent(const ThreadMsg* incoming) override
  { 
    ASSERT_TRUE(incoming->msg != NULL);

    auto userData = static_cast<const Msg*>(incoming->msg);  

    const DataMsg<UserDataA> *test = static_cast<const DataMsg<UserDataA>*>(userData);

    // std::cout << test->getPayload().msg.c_str() << std::endl
    //           << static_cast<int>(test->getPayload().evId) << std::endl
    //           << "on :" << this->getThreadName() << std::endl;

    ASSERT_TRUE((test->getPayload().evId >= ClassAEventIds::TASKA_MESSAGE_MIN) && (test->getPayload().evId < ClassAEventIds::TASKA_MESSAGE_MAX));

    sEventHandlersList[test->getPayload().evId](21);
    delete userData; 
  }

  virtual void ProcessTimerEvent(const ThreadMsg* incoming) override
  { 
    // OK I will send the user event to read the CAN bus  

    UserDataA* userData1 = new UserDataA();
    userData1->msg = "Read CAN BUS";
    userData1->evId = ClassAEventIds::TASKA_MESSAGE_READ_CAN_BUS; 

    DataMsg<UserDataA> *myData = new DataMsg<UserDataA>(userData1);

    this->PostMsg(myData);
    // Implement Notify() to inform other threads...
    // For this we need a list of observers.
  }

  void registerObserver(Observer *observer) {
    observers.push_back(observer);
}

void removeObserver(Observer *observer) {
    // find the observer
    auto iterator = std::find(observers.begin(), observers.end(), observer);

    if (iterator != observers.end()) { // observer found
        observers.erase(iterator); // remove the observer
    }
}

void notifyObservers() {

    float* val = new float(56.93);
    ObseverDataMsg<float> *data = new ObseverDataMsg<float>(val);
    for (Observer *observer : observers) { // notify all observers
        observer->update(data);
    }
}

};

// Definition for the static member!
EventMap A::sEventHandlersList; 

#endif 