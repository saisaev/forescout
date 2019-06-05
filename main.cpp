#include <thread>
#include <chrono>
#include <iostream>
#include <future>

#include "time.h"
#include "cronejob.h"
#include "scheduler.h"

#define LIFETIME_SECS 60 

void output(int id)
{
	std::cout << id << " ";
}

struct context_functor
{
	void operator()() {
		output(2);
	}
};

void time_tick(scheduler* scheduler, std::future<void> futureObj) {
	struct timeval timestamp;
	timestamp.tv_usec = 0;
	long t = 0;
	while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		timestamp.tv_sec = t;

		scheduler->OnNewTime(timestamp);
    	
		std::cout << " <" << t << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		t++;
	}
}

int main(int argc, char * argv[])
{
	scheduler scheduler;

	// run ´external´ timestamp supplier
	std::promise<void> exitSignal;
	std::future<void> futureObj = exitSignal.get_future();
	std::thread timer (time_tick, &scheduler, std::move(futureObj));

	// create 4 crone jobs
	cronejob job1(1);
	cronejob job2(2);
	cronejob job3(3);
	cronejob job4(4);

	// create 3 job contexts using functor, lambda and function
	context_functor context2;
	context context3 = std::bind(output, 3);
	context context4 = []() { output(4); };

	// add jobs to scheduler
	scheduler.add_job(job1);  // without a context; displays nothing
	scheduler.add_job(job2, context2);
	scheduler.add_job(job3, context3);
	scheduler.add_job(job4, context4);

	// proceed 30 secs with 4 jobs
	std::this_thread::sleep_for(std::chrono::seconds(LIFETIME_SECS / 2));

	scheduler.remove_job(job2);
	scheduler.remove_job(job3);
	
	// proceed the next 30 secs with 2 jobs only
	std::this_thread::sleep_for(std::chrono::seconds(LIFETIME_SECS / 2));
	// gracefully stop the timestamp supplier
	exitSignal.set_value();

	timer.join();
	return 0;
}

