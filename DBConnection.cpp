#include "DBConnection.h"
#include "DBConfigurator.h"
#include <QStringList>
#include <QDir>

DBConnection::DBConnection(const QString& connection_name)
	:database(QSqlDatabase::addDatabase("QODBC", connection_name))
{}

DBConnection::DBConnection(const QString& connection_config, const QString & connection_name)
	:DBConnection(connection_name)
{
	const auto config_path = QDir::current().absolutePath() + "/" + connection_config;
	DBConfigurator configurator(config_path);
	if (!configurator.getError())
	{
		QString connection_name = "DRIVER={SQL Server};";
		connection_name += "SERVER=" + configurator.getServer() + ";";
		connection_name += "PORT=" + configurator.getPort() + ";";
		connection_name += "DATABASE=" + configurator.getDatabaseName() + ";";

		QStringList connection_data{
			connection_name,
			configurator.getUsername(),
			configurator.getPassword()
		};

		connect(connection_data);
	}
	else
	{
		const auto error_message = configurator.getErrorMessage();
		throw std::runtime_error((error_message + config_path).toStdString());
	}
}

DBConnection::~DBConnection()
{
	if (database.isOpen())
	{
		database.close();
	}
}

void DBConnection::connect(const QStringList& connection_data)
{
	database.setDatabaseName(connection_data[0]);
	database.setUserName(connection_data[1]);
	database.setPassword(connection_data[2]);
	database.open();
}

bool DBConnection::isOpen() const
{
	return database.isOpen();
}

QSqlDatabase DBConnection::getDatabase() const
{
	return database;
}

QSqlError DBConnection::getLastError() const
{
	return database.lastError();
}
