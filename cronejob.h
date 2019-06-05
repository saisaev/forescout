#ifndef _JOB__
#define _JOB__

#include <functional>
#include "time.h"

typedef std::function<void()> context;

class cronejob
{
	int m_interval;
	struct timeval m_next_timestamp;

public:
	cronejob(int interval)
		: m_interval(interval)
		, m_next_timestamp {0, 0}
	{}

	int get_interval() { return m_interval; }
	void operator() (context context) { if (context) { context(); } }
	struct timeval get_next_timestamp() { return m_next_timestamp; }
	void set_next_timestamp(const timeval& timestamp) { m_next_timestamp = timestamp; }
};

#endif
