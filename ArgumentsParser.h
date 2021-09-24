#pragma once
#include <qobject.h>

class Receiver;
class Uploader;

class ArgumentsParser :
    public QObject
{
    QString data_source{};
    QString data_destination{};
public:
    ArgumentsParser(const QStringList& arguments);
    bool accepted() const;

    std::unique_ptr<Receiver> getReceiver() const;
    std::unique_ptr<Uploader> getUploader() const;

private:
    QString getDirection(const QStringList& arguments, const QString& argument_template);
};

