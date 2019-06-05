#include <algorithm>
#include "scheduler.h"

bool operator<=(struct timeval t0, struct timeval t1)
{
	return
		(t0.tv_sec < t1.tv_sec) ||
		((t0.tv_sec == t1.tv_sec) && (t0.tv_usec <= t1.tv_usec));
}

void scheduler::OnNewTime(const timeval& timestamp)
{
	std::lock_guard<std::mutex> lock(m_jobslocker);
	std::for_each(m_jobs.begin(), m_jobs.end(),
		[this, &timestamp](job_n_context& ref_pair) 
		{ 
			cronejob& job = ref_pair.first.get();
			auto next_timestamp = job.get_next_timestamp();
			if (next_timestamp <= timestamp) {
				struct timeval new_timestamp;
				new_timestamp.tv_sec = timestamp.tv_sec + job.get_interval();
				new_timestamp.tv_usec = timestamp.tv_usec;
				job.set_next_timestamp(new_timestamp);
				// execute job
				ref_pair.first(ref_pair.second);
			}
		}
	);
}

void scheduler::add_job(cronejob& job, context context)
{
	std::lock_guard<std::mutex> lock(m_jobslocker);
	struct timeval next = m_current_time;
	next.tv_sec += job.get_interval();
	job.set_next_timestamp(next);
	m_jobs.push_back(std::make_pair(std::ref(job), context));
}

void scheduler::remove_job(cronejob& removable_job)
{
	std::lock_guard<std::mutex> lock(m_jobslocker);
	m_jobs.erase(
		std::remove_if(m_jobs.begin(), m_jobs.end(), 
			[&removable_job](job_n_context& ref_pair) { return &removable_job == &(ref_pair.first.get()); })
		, m_jobs.end());
}

const int scheduler::size() { 
	std::lock_guard<std::mutex> lock(m_jobslocker);
	return m_jobs.size(); 
}

const bool scheduler::contains(cronejob& job) {
	std::lock_guard<std::mutex> lock(m_jobslocker);
	return m_jobs.end() != std::find_if(
		m_jobs.begin(), 
		m_jobs.end(), 
		[&job](job_n_context& ref_pair) 
		{ 
			return &job == &ref_pair.first.get();
		}
	);
}
