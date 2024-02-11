#include "Runnable.h"
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

typedef enum {
    INIT,
    RUNNING,
    STOP
} ThreadState;
// runnable class
class MyRunnable : public Runnable {
private:
    mutex mmutex;
    Condition* condition;
    ThreadState requestedState = INIT;
    ThreadState currentState = INIT;
    int id;
    std::function<void(int)> callback;

public:
    MyRunnable(int id)
    {
        this->id = id;
        condition = new Condition(mmutex);
    }
    ~MyRunnable()
    {
        delete condition;
    }
    void setCallback(std::function<void(int)> callback)
    {
        this->callback = callback;
        ;
    }
    int getId()
    {
        return id;
    }
    void setState(ThreadState nState) {
        { Synchronized x(mmutex);
    requestedState = nState;
    condition->notifyAll(x);
}
}
;
ThreadState getState()
{
    Synchronized x(mmutex);
    return currentState;
};
virtual void run()
{
    {
        Synchronized x(mmutex);
        currentState = RUNNING;
    }
    cout << "Thread " << id << " started" << endl;
    while (true) {
        {
            Synchronized x(mmutex);
            if (requestedState == STOP) {
                currentState = STOP;
                cout << "stopping thread " << id << endl;
                return;
            }
            condition->wait(x, 1000);
            if (callback) {
                callback(id);
            }
        }
        cout << "hello thread " << id << endl;
    }
}
}
;
void cb(int x)
{
    std::cout << "Callback " << x << std::endl;
}
int main(int argc, char** argv)
{
    cout << "hello world" << endl;
    int numThreads = 3;
    if (argc > 1) {
        numThreads = atoi(argv[1]);
    }
    vector<MyRunnable*> runnables;
    for (int i = 0; i < numThreads; i++) {
        MyRunnable* runnable = new MyRunnable(i);
        Thread* t = new Thread(runnable);
        // set callback function
        runnable->setCallback(cb);
        t->start();
        runnables.push_back(runnable);
    }
    //"misuse" of a condition for ms waits
    Condition waiter;
    // waiting for start
    bool allRunning = false;
    cout << "waiting for threads to start" << endl;
    while (!allRunning) {
        allRunning = true;
        vector<MyRunnable*>::iterator it = runnables.begin();
        while (it != runnables.end()) {
            if ((*it)->getState() != RUNNING) {
                cout << "##state " << ((*it)->getId()) << " is " << ((*it)->getState()) << endl;
                allRunning = false;
            }
            it++;
        }
        waiter.wait(20); // avoid busy loop
    }
    cout << numThreads << " are running" << endl;
    waiter.wait(10000);
    cout << "stopping" << endl;
    vector<MyRunnable*>::iterator it = runnables.begin();
    while (it != runnables.end()) {
        (*it)->setState(STOP);
        it++;
    }
    bool allStopped = false;
    int maxWait = 50 * 2; // max 2s
    while (!allStopped && maxWait) {
        allStopped = true;
        vector<MyRunnable*>::iterator it = runnables.begin();
        while (it != runnables.end()) {
            if ((*it)->getState() != STOP)
                allStopped = false;
            it++;
        }
        maxWait--;
        waiter.wait(20); // avoid busy loop
    }
    if (allStopped)
        cout << "all threads stopped" << endl;
    else
        cout << "unable to stop all threads" << endl;
    // if we would like to wait here for all threads
    // we also would need to keep the thread objects...
};
