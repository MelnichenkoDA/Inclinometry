#pragma once

#include <QString>
#include <QSQLDatabase>
#include <QSqlQuery>
#include <QSQLError>

class DBConnection
{
	QSqlDatabase database;
public:
	DBConnection(const QString& connection_name);
	DBConnection(const QString& connection_config, const QString & connection_name);
	~DBConnection();

	void connect(const QStringList & connection_data);

	bool isOpen() const;
	QSqlDatabase getDatabase() const;
	QSqlError getLastError() const;
};

