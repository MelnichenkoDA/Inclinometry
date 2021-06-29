#pragma once
#include <qobject.h>
#include <vector>
#include <qdatetime.h>

class WellInterval : public QObject
{
    Q_OBJECT
public:
    WellInterval();
    
private:
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

class Well :
    public QObject
{
public:
    Well();
    Well(size_t size);
    Well(const Well& other);

    bool isMinute = true;
    double wellid_;                                    //SK_1
    double magnite_deviation = 0;
    QString field_id_;                              //MS_1
    QString well_number_;                           //S1_1
    std::vector<double> interval_end_;              //GL_1
    std::vector<double> interval_begin_;            //GLN_1
    std::vector<double> tilt_angle_;                //UG_1
    std::vector<double> azimut_;                    //AZ_1
    std::vector<double> borehole_;                  //UL_1
    std::vector<double> abs_depth_;                 //ZZ_1
    std::vector<double> dx_;                        //XX_1
    std::vector<double> dy_;                        //YY_1
    std::vector<double> borehole_shift_;            //SM_1
    std::vector<double> directional_angle_;         //DU_1
    std::vector<double> crooked_ness_;              //KR_1
    QDate correction_time_;                          //TK_1    
    std::vector<double> vertical_depth_;            //GLV
};

