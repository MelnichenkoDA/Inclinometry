#pragma once
#include <qobject.h>
#include <vector>
#include <qdatetime.h>
#include "WellInterval.h"


class Well :
    public QObject
{
public:
    Well();
    Well(size_t size);
    Well(const Well& other);

    bool isMinute;
    double ois_code;                                      //SK_1
    double magnite_deviation;
    QString field_id;                                   //MS_1
    QString well_number;                                //S1_1
    QDate correction_time;                              //TK_1
    QString uwi;
    std::vector<WellInterval> intervals;
};

