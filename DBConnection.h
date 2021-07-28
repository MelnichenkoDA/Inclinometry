#pragma once

#include <QString>
#include <QSQLDatabase>
#include <QSqlQuery>
#include <QSQLError>

class DBConnection
{
	QSqlDatabase database;
public:
	DBConnection();
	DBConnection(const QStringList& connection_data);
	~DBConnection();

	void connect(const QStringList & connection_data);

	bool isOpen() const;
	QSqlDatabase getDatabase() const;
	QSqlError lastError() const;
};

