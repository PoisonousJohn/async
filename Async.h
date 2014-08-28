#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <boost/signals2.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>


/**
* @author John Poison
*/

namespace Async {

    class Work;

    class Async;

    class Worker {

    public:
        
        Worker();

        virtual ~Worker();

        size_t getQueueSize() const { return queue.size(); }

        void enqueue(Work* work);
        
    protected:
        std::vector< Work* > queue;
        
        std::vector< Work* > tasks;
        
        std::recursive_mutex workerMutex;

        std::condition_variable_any cv;

        std::atomic<bool> stop;

        std::thread thread;

    protected:
        
        void workHandler();
        
    };

    /**
    * @class Work represents a single task
    * that should be done on a thread
    */
    class Work final : public boost::noncopyable {

    public:

        /**
        * @param bgJob job that should done in a background thread\n
        * will be done on a background thread only if background = true\n
        *
        * otherwise will be done on main thread
        * @param manualStart if true work will be immediately enqueued for processing\n
        * otherwise job will be enqueued only after start() method
        *
        * @param fgNotificaton job that should be done after main (bgJob) has been completed\n
        * fgNotification job will be done always on the main thread
        *
        * @param threadIndex setting the thread index will make\n
        * the job to be done by concrete thread with\n
        * specified index. If index < 0 then optimal thread will be used
        */
        Work(
                Async* async,
                std::function<void()> bgJob,
                std::function<void()> fgNotification,
                bool manualStart = false,
                bool background = true,
                int threadIndex = -1
        );

        ~Work();

        /**
        * @brief will enqueue work
        */
        void start();

        /**
        */
        void setThreadIndex(unsigned value) {
            threadIndex.reset(value);
        }

        const boost::optional<unsigned>& getThreadIndex() const { return threadIndex; }

        /**
        * @return true if work is done and should be removed
        */
        virtual bool update();

        bool operator==(Work* rvalue) {
            return this == rvalue;
        }

        void run() {
            if (job)  {
                job();
                hasDone = true;
            }
        }


    private:

        boost::
        optional<unsigned>      threadIndex;

        std::atomic<bool>		hasDone;

        std::function<void()> 	fgNotification;

        std::function<void()> 	job;

        bool                    started;

        bool                    background;

        Async*                  async;

    };

    class Async {
        
    public: // signals
        
        boost::signals2::signal<void(Work*)> onWorkDone;
        
    public:
        
        Async();
        
        virtual ~Async();
        
        
        /**
         * @brief do on main thread
         */
        
        virtual Work* doSync( std::function<void()> job );
        
        /**
         * @brief do on one of background threads
         */
        virtual Work* doAsync( std::function<void()> bgJob, std::function<void()> fgNotification, bool manualWorkStart = false, int threadIndex = -1 );
        
        /**
         * @brief you should periodically invoke this method
         * to start enqueued work
         */
        virtual void update();

        void startWork(Work* work);

        void removeWork(Work* work);
        
        bool isMainThread() const;
        
        void setThreadsCount(unsigned value);
        
        size_t getThreadsCount() const;


    protected:
        
        std::thread::id mainThreadId;
        
        boost::ptr_vector<Worker> workers;
        
        boost::ptr_vector<Work> work;

        mutable std::recursive_mutex mutex;

    protected:

        Worker& getOptimalWorker();
    };
    

}
