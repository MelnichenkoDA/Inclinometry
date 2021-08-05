#pragma once
#include <qobject.h>
#include <qcoreapplication.h>
#include <qdebug.h>

class Receiver;
class Uploader;

class Task :
    public QObject
{
    Q_OBJECT
public:
    Task(QObject* parent = 0);

public slots:
    void run();

signals:
    void finished();

private:
    bool checkArguments();
    std::unique_ptr<Receiver> getReceiver();
    std::unique_ptr<Uploader> getUploader();
};

