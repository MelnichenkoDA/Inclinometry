#pragma once
#include <QFile>
#include <QMap>

class DBConfigurator
{
public:
	DBConfigurator(const QString& filename);

	const QString getDatabaseName() const;
	const QString getUsername() const;
	const QString getPassword() const;
	const QString getServer() const;
	const QString getPort() const;

	QFileDevice::FileError getError() const;
	QString getErrorMessage() const;
private:
	QMap<QString, QString> connection_fields
	{
		{"server", {}},
		{"port", {}},
		{"password", {}},
		{"username", {}},
		{"dbname", {}}
	};
	QFileDevice::FileError error;
};

