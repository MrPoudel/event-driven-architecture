
#ifndef THREAD_H_
#define THREAD_H_

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include "Observer.hpp"
#include <functional>
#include <map>

// This is
class Msg {
  public:
    virtual ~Msg() {}
};

template <typename PayloadType> class DataMsg : public Msg {
  public:
    DataMsg() {}
    DataMsg(PayloadType *pType) : pl_(pType) {}
    virtual ~DataMsg() {}

    PayloadType &
    getPayload() const {
        return *pl_;
    }

  private:
    PayloadType *pl_;
};

enum class ClassAEventIds;
enum class ClassBEventIds;

#pragma pack(push, 1)
struct ThreadMsg {
    ThreadMsg(int i, const void *m) {
        id = i;
        msg = m;
    }
    int id;
    const void *msg;
};
#pragma pack(pop)

class WorkerThread {
  public:
    /// Constructor
    WorkerThread(const char *threadName);

    /// Destructor
    ~WorkerThread();

    /// Called once to create the worker thread
    /// @return TRUE if thread is created. FALSE otherwise.
    bool
    CreateThread();

    /// Called once a program exit to exit the worker thread
    void
    ExitThread();

    /// Get the ID of this thread instance
    /// @return The worker thread ID
    std::thread::id
    GetThreadId();

    /// Get the ID of the currently executing thread
    /// @return The current thread ID
    static std::thread::id
    GetCurrentThreadId();

    /// Add a message to thread queue.
    /// @param[in] data - thread specific information created on the heap using
    /// operator new.
    /// template<typename T>
    void
    PostMsg(const Msg *data);

    virtual void
    ProcessUserEvent(const ThreadMsg *incoming) = 0;
    virtual void
    ProcessTimerEvent(const ThreadMsg *incoming) = 0;
    /**
     * Register an observer
     * @param observer the observer object to be registered
     */
    virtual void
    registerObserver(Observer *observer) {}

    /**
     * Unregister an observer
     * @param observer the observer object to be unregistered
     */
    virtual void
    removeObserver(Observer *observer) {}

    /**
     * Notify all the registered observers when a change happens
     */
    virtual void
    notifyObservers() {}

    const char *
    getThreadName() {
        return THREAD_NAME;
    }

    void
    ExitTimer() {
        m_timerExit = true;
        std::cout << "Timer on " << THREAD_NAME << " is going to be off." << std::endl;
    }

  private:
    WorkerThread(const WorkerThread &);
    WorkerThread &
    operator=(const WorkerThread &);

    /// Entry point for the worker thread
    void
    Process();

    /// Entry point for timer thread
    void
    TimerThread();  // TODO: This has to be a abstract method and implemented
    // while instantiating because not all the instances of threads may need
    // timer?!

    std::thread *m_thread;
    std::queue<ThreadMsg *> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_timerExit;
    const char *THREAD_NAME;
};

#endif /* THREAD_H_ */