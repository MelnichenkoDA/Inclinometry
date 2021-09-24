#pragma once
#include <qobject.h>
#include <QDate>

class QSqlQuery;

class WellInterval :
    public QObject
{
    Q_OBJECT
public:
    WellInterval();
    explicit WellInterval(const QSqlQuery& query);
    WellInterval(const WellInterval& other);

    //DKL_1
    double interval_end;              //GL_1
    double interval_begin;            //GLN_1
    double angle;                     //UG_1
    double azimut;                    //AZ_1
    double borehole;                  //UL_1
    double dx;                        //XX_1
    double dy;                        //YY_1
    double dz;                        //ZZ_1
    double displacement;              //SM_1
    double directional_angle;         //DU_1
    double curvature;                 //KR_1
    double vertical_depth;            //GLV
    QString curvature_max;            //KR_MAX
    double inclinometry;              //ZB2_1
    double geographic_azimut;         //GRAZ
    QDate measurement_date;           //DKL_1
};

