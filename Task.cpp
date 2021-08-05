#include "Task.h"
#include "XLSReceiver.h"
#include "ARMDataProcessor.h"

#include "XLSWrapper.h"
#include "ExcelUploader.h"

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

	try
	{
		
		auto data = getReceiver()->getData();
		
		ARMDataProcessor processor;
		processor.process(data);

		auto uploader = getUploader();
		if (uploader)
			uploader->uploadData(data);
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
			const auto& argument = application_arguments[1];
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

std::unique_ptr<Receiver> Task::getReceiver()
{
	auto application_arguments = QCoreApplication::arguments();
	const auto& application_argument = application_arguments[1];
	std::unique_ptr<Receiver> receiver{};

	if (application_argument.startsWith("-d="))
		receiver.reset(new XLSReceiver(application_argument.right(application_argument.size() - 3)));
	else if (application_argument.startsWith("-l="))
		//TODO
		receiver = nullptr;

	return receiver;
}

std::unique_ptr<Uploader> Task::getUploader()
{
	auto application_arguments = QCoreApplication::arguments();
	const auto& application_argument = application_arguments[1];
	std::unique_ptr<Uploader> uploader{};

	if (application_argument.startsWith("-d="))
	{
		auto filename = application_argument.right(application_argument.size() - 3).split('.')[0];		
		uploader.reset(new ExcelUploader(filename + "_processed" + ".xls"));
	}
	else if (application_argument.startsWith("-l="))
	{
		uploader = nullptr;
	}

	return uploader;
}
