#include "Task.h"
#include "XLSReceiver.h"
#include "DataProcessor.h"


Task::Task(QObject* parent)
	: QObject(parent)
{}

void Task::run()
{
	if (checkArguments())
	{
		qInfo() << "Wrong command line arguments' format!";
		std::exit(0);
	}

	auto receiver = getReceiver();
	try {
		auto data = receiver->getData();
		ARMDataProcessor processor;
		processor.process(data);
	}
	catch (const std::exception& exc)
	{
		qInfo() << exc.what();
		std::exit(0);
	}
	

}


//return true if error
bool Task::checkArguments()
{
	auto application_arguments = QCoreApplication::arguments();
	if (application_arguments.size() > 1)
	{
		if (application_arguments.size() > 2)
		{
			return true;
		}
		else
		{
			const auto & argument = application_arguments[1];
			if (argument.startsWith("-d="))
				return false;
			else if (argument.startsWith("-l="))
				return false;
			else
				return true;
		}		
	}
	return true;
}

Receiver* Task::getReceiver()
{
	auto application_arguments = QCoreApplication::arguments();
	const auto& application_argument = application_arguments[1];
	Receiver* receiver{};
	
	if (application_argument.startsWith("-d="))
		receiver = new XLSReceiver(application_argument.right(application_argument.size() - 3));
	else if (application_argument.startsWith("-l="))
		//TODO
		receiver = nullptr;
		

	return receiver;
}
