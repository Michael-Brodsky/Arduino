#include "TaskScheduler.h"

#pragma region TaskScheduler
TaskScheduler::TaskScheduler(Task* tasks[], size_t n) : 
	tasks_(tasks, n) 
{

}

TaskScheduler::TaskScheduler(Task** first, Task** last) : 
	tasks_(first, last) 
{

}

void TaskScheduler::tasks(Task** first, Task** last)
{
	tasks_ = ArrayWrapper<Task*>(first, last);
}

void TaskScheduler::tick()
{
	for (iterator task = std_begin(tasks_); task < std_end(tasks_); ++task)
	{
		if (scheduled(task))
		{
			(*task)->last_ = millis();
			(*task)->command_->execute();
		}
	}
}

bool TaskScheduler::scheduled(const_iterator task) const
{
	return !((*task)->state_ == Task::State::Idle || (millis() - (*task)->last_ < (*task)->interval_));
}
#pragma endregion
#pragma region Task
TaskScheduler::Task::Task(ICommand* command, msecs_t interval, State state) :
	command_(command), interval_(interval), last_(), state_(state)
{
	assert(command);
}

ICommand* TaskScheduler::Task::command() 
{ 
	return command_; 
}

const ICommand* TaskScheduler::Task::command() const 
{ 
	return command_; 
}

msecs_t& TaskScheduler::Task::interval() 
{ 
	return interval_; 
}

const msecs_t& TaskScheduler::Task::interval() const 
{ 
	return interval_; 
}

TaskScheduler::Task::State& TaskScheduler::Task::state() 
{ 
	return state_; 
}

const TaskScheduler::Task::State& TaskScheduler::Task::state() const 
{ 
	return state_; 
}

bool TaskScheduler::Task::operator==(const Task& other) const 
{
	return this->command_ == other.command_;
}
#pragma endregion