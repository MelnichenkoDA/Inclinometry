#include "DBConfigurator.h"
#include <QTextStream>

DBConfigurator::DBConfigurator(const QString& filename)
    :error{}
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        error = file.error();
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        auto line = in.readLine();
        auto fields = line.split(":");
        if (fields.size())
        {
            if (auto itr = connection_fields.find(fields[0]); itr != connection_fields.end())
                itr.value() = fields[1];
            else
                error = QFileDevice::ReadError;
        }
        else
        {
            error = QFileDevice::ReadError;
        }

        if (error)
            break;
    }

    file.close();
}

QFileDevice::FileError DBConfigurator::getError() const
{
    return error;
}

QString DBConfigurator::getErrorMessage() const
{
    switch (error)
    {
    case QFileDevice::ReadError:
        return "An error occured when reading from the file: ";    
    case QFileDevice::FatalError:
        return "A fatal error occured during working with the file: ";
    case QFileDevice::ResourceError:
        return "Out of resources during working with the file: ";
    case QFileDevice::OpenError:
        return "Could not open the file: ";
    case QFileDevice::AbortError:
        return "The operation was aborted during working with the file: ";
    case QFileDevice::TimeOutError:
        return "A timeout occurred during working with the file: ";
    case QFileDevice::UnspecifiedError:
        return "An unspecified error occurred during working with the file: ";
    case QFileDevice::PermissionsError:
        return "Couldn't get acces to the file: ";
    default:
        return "Another error occured during working with the file: ";
    }
}

const QString DBConfigurator::getDatabaseName() const
{
    return connection_fields["dbname"];
}

const QString DBConfigurator::getUsername() const
{
    return connection_fields["username"];
}

const QString DBConfigurator::getPassword() const
{
    return connection_fields["password"];
}

const QString DBConfigurator::getServer() const
{
    return connection_fields["server"];
}

const QString DBConfigurator::getPort() const
{
    return connection_fields["port"];
}
