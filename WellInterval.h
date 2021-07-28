#pragma once
#include <qobject.h>
#include <QSqlQuery>

class WellInterval :
    public QObject
{
    Q_OBJECT
public:
    WellInterval();
    explicit WellInterval(const QSqlQuery& query);
    WellInterval(const WellInterval& other);

    double interval_end;              //GL_1
    double interval_begin;            //GLN_1
    double angle;                     //UG_1
    double azimut;                    //AZ_1
    double borehole;                  //UL_1
    double abs_depth;                 //ZZ_1
    double dx;                        //XX_1
    double dy;                        //YY_1
    double displacement;              //SM_1
    double directional_angle;         //DU_1
    double curvature;                 //KR_1
    double vertical_depth;            //GLV
};

