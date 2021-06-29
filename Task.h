#pragma once
#include <qobject.h>
#include <qcoreapplication.h>
#include <qdebug.h>

#include "Receiver.h"

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
    Receiver* getReceiver();
};

