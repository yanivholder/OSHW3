#ifndef __THREAD_H
#define __THREAD_H

//#include "../Part1/Headers.hpp"
//#include "../Part1/PCQueue.hpp"
#include "Args.h"

class Thread
{
public:
    // Only places thread_id
    Thread(uint thread_id):
	        m_thread_id(thread_id){}

	virtual ~Thread() {} // Does nothing 

	/** Returns true if the thread was successfully started, false if there was an error starting the thread */
	// Creates the internal thread via pthread_create 
    bool start(void (*the_func)(bool, int, int), PCQueue<job>& pcq,
                int num_of_gens, vector<double>& m_tile_hist, Semaphore& sem)
	{
        GOL_Args args(the_func, pcq, num_of_gens, m_tile_hist, sem);
        void* (*start_routine)(void*) = &Thread::thread_workload;
        int res = pthread_create(&this->m_thread, nullptr, start_routine, (void*)&args);
        return res==0? 0 : true;
    }

	/** Will not return until the internal thread has exited. */
	void join()
	{
	    pthread_join(m_thread, nullptr);
	}

	/** Returns the thread_id **/
	uint thread_id()
	{
		return m_thread_id;
	}
private:
    uint m_thread_id; // A number from 0 -> Number of threads initialized, providing a simple numbering for you to use
    pthread_t m_thread;

	static void* thread_workload(void* args) {
        GOL_Args my_args = *((GOL_Args*)args);
        for (int i = 0; i < 2 * my_args.num_of_gens; ++i) {
            job my_job = my_args.assignments.pop();

            auto start_time = std::chrono::high_resolution_clock::now();
            my_args.func(my_job.is_first_phase, my_job.upper_row, my_job.lower_row);
            auto end_time = std::chrono::high_resolution_clock::now();

            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            my_args.m_tile_hist.insert(elapsed_time);

            my_args.s.up();
        }
        pthread_exit(nullptr);
    }
};

#endif
