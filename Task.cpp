#include "Task.h"
#include "XLSReceiver.h"
#include "ARMDataProcessor.h"

#include "XLSWrapper.h"
#include "DBUploader.h"
#include "ArgumentsParser.h"


Task::Task(QObject* parent)
	: QObject(parent)
{}

void Task::run()
{
	try
	{
		ArgumentsParser parser(QCoreApplication::arguments());
		if (parser.accepted())
		{
			if (auto receiver = parser.getReceiver(); receiver)
			{
				auto data = receiver->getData();
				ARMDataProcessor processor;
				processor.process(data);

				auto uploader = parser.getUploader();
				if (uploader)
					uploader->uploadData(data);
			}
		}		
	}
	catch (const std::exception& exc)
	{
		qInfo() << exc.what();
		std::exit(0);
	}
}
