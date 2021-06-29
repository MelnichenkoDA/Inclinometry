#include "ARMDataProcessor.h"

ARMDataProcessor::ARMDataProcessor()
{}

void ARMDataProcessor::process(std::vector<Well>& isoho_wells) const
{
    for (auto & well : isoho_wells)
    {
        for (unsigned i = 0; i < well.interval_end_.size(); ++i)
        {
            if (fabs(ceil(well.tilt_angle_[i] * 1e3f) - floor(well.tilt_angle_[i])) >= 0.6 ||
                fabs(ceil(well.azimut_[i] * 1e3f) - floor(well.azimut_[i])) >= 0.6)
            {
                well.isMinute = false;
                break;
            }
        }

        if (well.isMinute)
        {
            double split{}, integer{};
            for (unsigned i = 0; i < well.interval_end_.size(); ++i)
            {
                split = modf(well.tilt_angle_[i], &integer);
                well.tilt_angle_[i] = integer + split / 60.f * 100.f;

                split = modf(well.azimut_[i], &integer);
                well.azimut_[i] = integer + split / 60.f * 100.f;               
            }
        }

        for (unsigned i = 1; i < well.interval_end_.size(); ++i)
        {
            const auto prev_angle = well.tilt_angle_[i - 1];
            const auto angle = well.tilt_angle_[i];
            const auto prev_azimut = well.azimut_[i - 1];
            const auto azimut = well.azimut_[i];

            const auto interval = well.interval_end_[i] - well.interval_end_[i - 1];
            well.dy_[i] = well.dy_[i - 1] + interval * mySin(prev_angle, angle) * myCos(prev_azimut, azimut) - well.dy_[0];
            well.dx_[i] = well.dx_[i - 1] + interval * mySin(prev_angle, angle) * mySin(prev_azimut, azimut) - well.dx_[0];
            well.abs_depth_[i] = well.abs_depth_[i - 1] + interval * myCos(prev_angle, prev_azimut) - well.abs_depth_[0];
            well.borehole_[i] = well.interval_end_[i] - well.abs_depth_[i] + well.abs_depth_[0];
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
