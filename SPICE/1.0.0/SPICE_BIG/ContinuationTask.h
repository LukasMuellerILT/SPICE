/**
	SPICE BIG
	ContiunationTask.h
	Purpose: A continuation task which can be generated by a command and propsen to the PMS in case of an error

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef CONTINUATIONTASK_H
#define CONTINUATIONTASK_H

#include <string>

namespace SPICE
{
	namespace BIG
	{
		class ContinuationTask
		{
		public:
			enum TaskTypes
			{
				continueCommand,
				abortCurrentCommand,
				abortAllCommands,
				fatalError
			};

			/**
				To throw a generated error and maybe trigger the sending of an error event. 

				@param name Name of the task
				@param description Human readable task description
				@param type Type of the continuation task. Influences the impact to the state machine if this task is selected.
			*/
			ContinuationTask(std::string name, std::string description, ContinuationTask::TaskTypes type);
			virtual ~ContinuationTask(void);

			/**
				Gets the name of the continuation task

				@return Name of the task
			*/
			std::string getName();
			/**
				Gets the task description

				@return Human readable task description
			*/
			std::string getDescription();
			/**
				Gets the type of the continuation task

				@return Type of the continuation task
			*/
			TaskTypes getType();

		private:
			std::string _taskName;
			std::string _taskDescription;
			ContinuationTask::TaskTypes _taskType;
		};
	}
}

#endif // CONTINUATIONTASK_H