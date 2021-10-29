#include "ArgumentsParser.h"
#include "XLSReceiver.h"
#include "QueryReceiver.h"
#include "DBUploader.h"
#include "ExcelUploader.h"

namespace
{
	const QString kSourceDirection{ "-src=" };
	const QString kDestinationDirection{ "-dst=" };
	const QString kDefaultValue{ "def" };
}

ArgumentsParser::ArgumentsParser(const QStringList& arguments)
	:data_source{},
	data_destination{}
{
	const auto arguments_count = arguments.size();
	if (arguments_count == 1)
	{
		data_source = data_destination = kDefaultValue;		
	}
	else if (arguments_count <= 3)
	{
		data_source = getDirection(arguments, kSourceDirection);
		data_destination = getDirection(arguments, kDestinationDirection);
	}
}

bool ArgumentsParser::accepted() const
{
	if (!data_source.isEmpty() && !data_destination.isEmpty())
	{
		return true;
	}

	return false;
}

std::unique_ptr<Receiver> ArgumentsParser::getReceiver() const
{
	if (data_source == kDefaultValue)
	{
		return std::make_unique<QueryReceiver>();
	}
	else if (!data_source.isEmpty())
	{
		return std::make_unique<XLSReceiver>(data_source);
	} 
	else
	{
		return {};
	}
}

std::unique_ptr<Uploader> ArgumentsParser::getUploader() const
{
	if (data_destination == kDefaultValue)
	{
		return std::make_unique<DBUploader>();
	}
	else if (!data_destination.isEmpty())
	{
		return std::make_unique<ExcelUploader>(data_destination);
	}
	else
	{
		return {};
	}
}

QString ArgumentsParser::getDirection(const QStringList& arguments, const QString& argument_template)
{
	for (int i = 1; i < arguments.size(); ++i)
	{
		if (arguments[i].startsWith(argument_template))
		{
			if (const auto split = arguments[i].split(argument_template); split.size())
			{
				return split[1];
			}
			else
			{
				return QString();
			}
		}
	}
	
	return kDefaultValue;
}
