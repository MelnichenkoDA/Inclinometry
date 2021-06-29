#include <qapplication.h>
#include <QTimer>

#include "Task.h"

int main(int argc, char *argv[])
{
    QApplication  app(argc, argv);
    Task* task = new Task(&app);   
    QObject::connect(task, SIGNAL(finished()), &app, SLOT(quit()));
    QTimer::singleShot(60, task, SLOT(run()));    
    return app.exec();
}
