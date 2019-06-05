// <--include BoostTest
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CronejobTest
#include <boost/test/unit_test.hpp>
// -->

#include "../cronejob.h"

BOOST_AUTO_TEST_SUITE (cronejob_suite)

BOOST_AUTO_TEST_CASE (constructor_get_interval)
{
    int interval = 123;
	cronejob job(interval);

    BOOST_CHECK_EQUAL(job.get_interval(), interval);
}

BOOST_AUTO_TEST_CASE (set_get_next_timestamp)
{
    struct timeval expected({11, 22});
	cronejob job(0);
    job.set_next_timestamp(expected);

    auto actual = job.get_next_timestamp();
    BOOST_CHECK_EQUAL(actual.tv_sec, expected.tv_sec);
    BOOST_CHECK_EQUAL(actual.tv_usec, expected.tv_usec);
}

BOOST_AUTO_TEST_SUITE_END();
