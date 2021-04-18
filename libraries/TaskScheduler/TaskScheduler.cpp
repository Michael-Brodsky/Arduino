#include "library.h"		// `millis()'
#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(Task* tasks[], size_t size) : 
	begin_(tasks), end_(tasks + size)
{
}

TaskScheduler::TaskScheduler(iterator begin, iterator end) : 
	begin_(begin), end_(end)
{
}

void TaskScheduler::state(Task* task, Task::State state)
{
	auto it = std_find(begin_, end_, task);

	if(it != end_)
		(*it)->state_ = state;
}

void TaskScheduler::state(Task* task, bool active)
{
	TaskScheduler::Task::State task_state = (active)
		? TaskScheduler::Task::State::Active
		: TaskScheduler::Task::State::Idle;

	state(task, task_state);
}

TaskScheduler::Task::State TaskScheduler::state(const Task* task) const 
{
	auto it = std_find(begin_, end_, task);

	return it != end_ ? (*it)->state_ : Task::State::Idle;
}

bool TaskScheduler::scheduled(const Task* task) const 
{
	return !(task->state_ == Task::State::Idle || (millis() - task->last_ < task->interval_));
}

void TaskScheduler::tick()  
{
	for (auto it = begin_; it < end_; it++)
	{
		if (scheduled(*it))
		{
			(*it)->last_ = millis();
			(*it)->command_.execute();
		}
	}
}
