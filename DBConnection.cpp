#include "DBConnection.h"
#include <QStringList>

DBConnection::DBConnection()
	:database(QSqlDatabase::addDatabase("QOCI"))
{}

DBConnection::DBConnection(const QStringList& connection_data)
	:database(QSqlDatabase::addDatabase("QOCI"))
{
	connect(connection_data);
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

QSqlError DBConnection::lastError() const
{
	return database.lastError();
}
