// <--include BoostTest
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CronejobTest
#include <boost/test/unit_test.hpp>
// -->

#include "../cronejob.h"
#include "../scheduler.h"

BOOST_AUTO_TEST_SUITE (scheduler_suite)

BOOST_AUTO_TEST_CASE (add_remove_jobs)
{
	scheduler scheduler;

	cronejob job1(4);
	cronejob job2(2);
	cronejob job3(3);
	cronejob job4(1);

	scheduler.add_job(job1);
	scheduler.add_job(job2);

    BOOST_CHECK (2 == scheduler.size());
    BOOST_CHECK (scheduler.contains(job1));
    BOOST_CHECK (scheduler.contains(job2));

	scheduler.remove_job(job1);
	scheduler.remove_job(job2);

    BOOST_CHECK (0 == scheduler.size());
    BOOST_CHECK (!scheduler.contains(job1));
    BOOST_CHECK (!scheduler.contains(job2));

	scheduler.add_job(job3);
	scheduler.add_job(job4);

    BOOST_CHECK (2 == scheduler.size());
    BOOST_CHECK (scheduler.contains(job3));
    BOOST_CHECK (scheduler.contains(job4));
}

void trace_job(int interval)
{
	scheduler scheduler;

    bool job_executed = false;

    context context = [&job_executed]() { job_executed = true; };
	cronejob job(interval);
	scheduler.add_job(job, context);

    for (int i = 1; i < 100; i++)
    {
        scheduler.OnNewTime(timeval({i, 0}));

        auto expected = 0 == (i % interval);
        BOOST_CHECK (job_executed == expected);
        
        job_executed = false;
    }
}

BOOST_AUTO_TEST_CASE (on_new_time_job_interval_2)
{
    trace_job(2);
}

BOOST_AUTO_TEST_CASE (on_new_time_job_interval_7)
{
    trace_job(7);
}

BOOST_AUTO_TEST_SUITE_END();
