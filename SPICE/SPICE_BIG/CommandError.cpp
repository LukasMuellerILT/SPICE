/**
	SPICE BIG
	CommandError.cpp
	Purpose: An error which can be generated by a command.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include <sstream>

#include "CommandError.h"

#include "SiLASOAPFunctions.h"

namespace SPICE
{
	namespace BIG
	{
		CommandError::CommandError(std::string errorMessage) :
			_isRecoverable(false),
			_errorMessage(errorMessage),
			_automaticAfterTimeout(false),
			_userInteractionTimeout(0),
			_selectedTaskName(""),
			_selectedTaskType(ContinuationTask::TaskTypes::fatalError)
		{
			//ctor
		}
		CommandError::CommandError(std::string errorMessage, double userInteractionTimeout) :
			_isRecoverable(false),
			_errorMessage(errorMessage),
			_automaticAfterTimeout(true),
			_userInteractionTimeout(userInteractionTimeout),
			_selectedTaskName(""),
			_selectedTaskType(ContinuationTask::TaskTypes::fatalError)
		{
			//ctor
		}
		CommandError::CommandError(std::string errorMessage, std::string taskName, std::string taskDescription, ContinuationTask::TaskTypes taskType) :
			_isRecoverable(false),
			_errorMessage(errorMessage),
			_automaticAfterTimeout(false),
			_userInteractionTimeout(0),
			_selectedTaskName(""),
			_selectedTaskType(ContinuationTask::TaskTypes::fatalError)
		{
			addContinuationTask(taskName, taskDescription, taskType);
		}
		CommandError::CommandError(std::string errorMessage, double userInteractionTimeout, std::string taskName, std::string taskDescription, ContinuationTask::TaskTypes taskType) :
			_isRecoverable(false),
			_errorMessage(errorMessage),
			_automaticAfterTimeout(true),
			_userInteractionTimeout(userInteractionTimeout),
			_selectedTaskName(""),
			_selectedTaskType(ContinuationTask::TaskTypes::fatalError)
		{
			addContinuationTask(taskName, taskDescription, taskType);
		}
		CommandError::~CommandError()
		{
			//dtor
		}

		bool CommandError::addContinuationTask(std::string name, std::string description, ContinuationTask::TaskTypes type)
		{
			if(name == "")
			{
				return false;
			}

			for(unsigned int i = 0; i < _continuationTasks.size(); i++)
			{
				if(_continuationTasks[i].getName() == name)
				{
					return false;
				}
			}

			ContinuationTask newContinuationTask(name, description, type);
			_continuationTasks.push_back(newContinuationTask);

			if(type == ContinuationTask::TaskTypes::continueCommand || _continuationTasks.size() > 1)
			{
				_isRecoverable = true;
			}

			return true;
		}

		std::string CommandError::getXMLContinuationTaskParameter()
		{
			std::ostringstream strout;

			strout << "<ErrorMessage>" << _errorMessage << "</ErrorMessage>";

			for(unsigned int i = 0; i < _continuationTasks.size(); i++)
			{
				ContinuationTask contTask = _continuationTasks[i];
				if(i == 0)
				{
					strout << "<DefaultTask AutomaticAfterTimeout=\"";
					if(_automaticAfterTimeout)
					{
						strout << "true";
					}
					else
					{
						strout << "false";
					}
					strout << "\"><TaskType>" << contTask.getName() << "</TaskType><TaskText>" <<
						contTask.getDescription() << "</TaskText><UserInteractionTimeout>" <<
						SPICE::BIG::SiLASOAPFunctions::getDurationFromDoubleSeconds(_userInteractionTimeout) << "</UserInteractionTimeout></DefaultTask>";
				}
				else
				{
					strout << "<Task><TaskType>" <<
						contTask.getName() << "</TaskType><TaskText>" <<
						contTask.getDescription() << "</TaskText></Task>";
				}
			}

			return strout.str();
		}

		bool CommandError::isRecoverable()
		{
			return _isRecoverable;
		}

		unsigned int CommandError::getContTasksCount()
		{
			return _continuationTasks.size();
		}

		std::string CommandError::getErrorMessage()
		{
			return _errorMessage;
		}

		void CommandError::setSelectedTask(std::string taskName)
		{
			for(unsigned int i = 0; i < _continuationTasks.size(); i++)
			{
				if(_continuationTasks[i].getName() == taskName)
				{
					_selectedTaskName = taskName;
					_selectedTaskType = _continuationTasks[i].getType();
				}
			}
		}

		std::string CommandError::getSelectedTaskName()
		{
			return _selectedTaskName;
		}

		ContinuationTask::TaskTypes CommandError::getSelectedTaskType()
		{
			return _selectedTaskType;
		}

		void CommandError::setDefaultTaskAsSelectedTask()
		{
			if(_continuationTasks.size() > 0)
			{
				_selectedTaskName = _continuationTasks[0].getName();
				_selectedTaskType = _continuationTasks[0].getType();
			}
			else
			{
				_selectedTaskName = "";
			}
		}
	}
}
