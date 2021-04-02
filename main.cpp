#include <windows-service/windows-service.hpp>

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <string>
#include <string_view>

// #TODO: Write this example in a way that uses less meaningless threads

class ExampleApp;

// some meaningless work
void MeaninglessWork()
{
    const int32_t x = 420;
    const int32_t y = 69;

    for (int32_t i = 0; i <= 420; ++i)
    {
        std::cout << i << ": " << x * y * i << std::endl;
    }
}

/**
* This example application spins up a thread, 
*
* It implements the implicit interface required by Service class.
*
* Can be used as a template for your own applications.
*/
class ExampleApp
{
public:
    void work()
    {
        std::cout << "do the work, spin up threads, whatever." << std::endl;
        std::thread work_thread(MeaninglessWork);

        Sleep(1);
        std::cout << "wait for the work to be done" << std::endl;
        work_thread.join();
        
        std::cout << "finished some work" << std::endl;


        // The following code blocks are different ways you can end your start_work()

        ////////////////
        // The first example makes this process do the work once (how we got here) and immediately end the service thread/process (by not trying to work again)
        // In order to test this without running as a service, remove the Sleep in the main function of this program between app->run() and app->stop(), otherwise you'll wait no matter what
        
        // {
        //     std::unique_lock<std::mutex> lock(mtx);
        //     wait_to_work = false;
        //     should_work_again = false;
        //     lock.unlock();
        //     return;
        // }
        
        ////////////////
        // The second example loops the work as fast as it can be done but still allowing waiting for the stop signal
        // In order to test this without running as a service, add a Sleep in the main function of this program between app->run() and app->stop()
        
        // {
        //     wait_to_work = false;    
        //     return;
        // }

        /////////////////
        // The third example kicks off a thread that sends a work signal to work again after 5 seconds
        // In order to test this without running as a service, add a Sleep in the main function of this program between app->run() and app->stop()
        
        {      
            wait_to_work = true;
        
            if (!signal_thread.joinable() && should_work_again)
            {
                std::cout << "kicking off a signal thread" << std::endl;
                signal_thread = std::thread(&ExampleApp::signal_thread_func, this);
            }
            return;
        }
    }

    void work_loop()
    {
        while (should_work_again && !stopped)
        {
            std::cout << "work thread starting work" << std::endl;

            work();

            
            if (wait_to_work)
            {
                std::cout << "waiting to get signaled when we should no longer wait to do the work again" << std::endl;

                std::unique_lock<std::mutex> lock(mtx);
                sleeper.wait(lock, [this] { return !wait_to_work || stopped; });

                std::cout << "sleeper woken, work thread loop over, attempting to work again" << std::endl;
            }
        }
    }

    void work_signal()
    {
        if (stopped)
        {
            std::cout << "got signal to work again, but stopped, bailing" << std::endl;
            return;
        }

        if (!should_work_again)
        {
            std::cout << "got signal to work again, but shouldn't work again, bailing" << std::endl;
            return;
        }

        std::cout << "got signal to work again, waiting for mutex" << std::endl;
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "wait_to_work = false; sleeper being notified" << std::endl;
        wait_to_work = false;
        sleeper.notify_all();
    }

    void signal_thread_func()
    {
        if(should_work_again && !stopped)
        {
            std::cout << "signal thread sleeping" << std::endl;

            Sleep(5000);

            std::cout << "signal thread signaling to work again" << std::endl;

            work_signal();
        }
    }

    int run()
    {
        work_thread = std::thread(&ExampleApp::work_loop, this);
        return state();
    }

    void stop()
    {
        std::cout << "got signal to stop, waiting for work thread and mutex before stopping" << std::endl;

        std::unique_lock<std::mutex> lock(mtx);

        std::cout << "stopping" << std::endl;
        stopped = true;
        wait_to_work = false;
        should_work_again = false;
        lock.unlock();


        sleeper.notify_all();
        

        std::cout << "joining signal and work threads" << std::endl;

        if (signal_thread.joinable())
        {
            signal_thread.join();
        }
        std::cout << "signal thread joined" << std::endl;

        if (work_thread.joinable())
        {
            work_thread.join();
        }
        std::cout << "work thread joined" << std::endl;

        std::cout << "stop function now doing nothing and simply returning, allowing code execution to fall through" << std::endl;
    }

    //@return 0 for A_OK and anything else for failures.
    int state() const { return EXIT_SUCCESS; }

private:
    mutable std::mutex mtx;
    std::condition_variable sleeper;
    bool stopped = false;
    bool wait_to_work = true;
    bool should_work_again = true;

    std::thread work_thread;
    std::thread signal_thread;
};

#define SERVICENAME TEXT("WindowsServiceExample")
#define DISPLAYNAME TEXT("Windows Service Example") //displayed in Windows Services
#define DESCRIPTION TEXT("An example Windows Service implementation")

int __cdecl _tmain(int argc, TCHAR *argv[])
{
    std::cout << argv[argc-1] << std::endl;

    // If "run" is passed as an arg, just run our app code
    if (std::string(argv[argc-1]) == "run")
    {
        auto app = std::make_unique<ExampleApp>();
        app->run();

        // if you want to test your service running then getting a stop signal after 10 seconds
        // without running as a service, here is how you can do so
        Sleep(20000);

        app->stop();
        return EXIT_SUCCESS;
    }

    ServiceMain<ExampleApp>(SERVICENAME, DISPLAYNAME, DESCRIPTION, argv);
    return EXIT_SUCCESS;
}
