#include "ARMDataProcessor.h"

namespace
{
    double kNullValue = -999.25;
    double kEpsilon = 0.0001;
}

ARMDataProcessor::ARMDataProcessor()
{}

void ARMDataProcessor::process(std::vector<Well>& isoho_wells) const
{
    for (auto & well : isoho_wells)
    {
        for (const auto & interval : well.intervals)
        {
            if (fabs(ceil(interval.angle * 1e3f) - floor(interval.angle)) >= 0.6 ||
                fabs(ceil(interval.azimut * 1e3f) - floor(interval.azimut)) >= 0.6)
            {
                well.isMinute = false;
                break;
            }
        }

        if (well.isMinute)
        {
            double split{}, integer{};
            for (auto& interval : well.intervals)
            {
                split = modf(interval.angle, &integer);
                interval.angle = integer + split / 60.f * 100.f;

                split = modf(interval.azimut, &integer);
                interval.azimut = integer + split / 60.f * 100.f;
            }
        }

        for (unsigned i = 1; i < well.intervals.size(); ++i)
        {
            const auto prev_angle = well.intervals[i - 1].angle;
            const auto angle = well.intervals[i].angle;
            const auto prev_azimut = well.intervals[i - 1].azimut;
            const auto azimut = well.intervals[i].azimut;

            const auto interval = well.intervals[i].interval_end - well.intervals[i - 1].interval_end;
            
            if (isValidValue(azimut) && isValidValue(prev_azimut))
            {
                well.intervals[i].dy = well.intervals[i - 1].dy + interval * mySin(prev_angle, angle) * myCos(prev_azimut, azimut) - well.intervals[0].dy;
                well.intervals[i].dx = well.intervals[i - 1].dx + interval * mySin(prev_angle, angle) * mySin(prev_azimut, azimut) - well.intervals[0].dx;
                well.intervals[i].abs_depth = well.intervals[i - 1].abs_depth + interval * myCos(prev_angle, prev_azimut) - well.intervals[0].abs_depth;
                well.intervals[i].borehole = well.intervals[i].interval_end - well.intervals[i].abs_depth + well.intervals[0].abs_depth;
            }            
        }
    }
}

double ARMDataProcessor::mySin(double a, double b) const
{
    auto temp = grad * (a + b) / 2;
    if (abs(a - b) > 180)
        return -sin(temp);
    else
        return sin(temp);
}

double ARMDataProcessor::myCos(double a, double b) const
{
    auto temp = grad * (a + b) / 2;
    if (abs(a - b) > 180)
        return -cos(temp);
    else
        return cos(temp);
}

bool ARMDataProcessor::isValidValue(double a) const
{
    return abs(a - kNullValue) > kEpsilon;
}
