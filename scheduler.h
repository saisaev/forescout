#ifndef __SCHEDULER__
#define __SCHEDULER__

#include <vector>
#include <mutex>

#ifdef _WIN32
#include "time.h"
#else
#include <sys/time.h>
#endif

#include "time.h"
#include "cronejob.h"

typedef std::pair<std::reference_wrapper<cronejob>, context> job_n_context;

class scheduler
{
protected:
	std::vector<job_n_context > m_jobs;
	struct timeval m_current_time;
	std::mutex m_jobslocker;

public:
	scheduler(): m_current_time({0,0}){}

	void OnNewTime(const timeval&);

	void add_job(cronejob&, context context = nullptr);
	void remove_job(cronejob& );

    const int size();
    const bool contains(cronejob&);
};

#endif