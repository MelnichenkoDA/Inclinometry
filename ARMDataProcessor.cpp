#include "ARMDataProcessor.h"

namespace
{
	const double kNullValue = -999.25;
	const double kEpsilon = 0.0001;
	const double kGrad = 0.017453292519943295769236907684886;
	const double kRad = 57.295779513082320876798154814105;

	double mySin(double a, double b)
	{
		auto temp = kGrad * (a + b) / 2;
		if (abs(a - b) > 180)
			return -sin(temp);
		else
			return sin(temp);
	}

	double myCos(double a, double b)
	{
		auto temp = kGrad * (a + b) / 2;
		if (abs(a - b) > 180)
			return -cos(temp);
		else
			return cos(temp);
	}

	double SinHD(double a, double b)
	{
		auto temp = a - b;
		return sin(temp > 180 ? ((temp + 360.0) / 2.0 * kGrad) : (temp / 2.0 * kGrad));
	}

	double SinHS(double a, double b)
	{
		auto temp = a + b;
		return sin(a - b > 180 ? ((temp + 360.0) / 2.0 * kGrad) : (temp / 2.0 * kGrad));
	}
}

ARMDataProcessor::ARMDataProcessor()
{}

void ARMDataProcessor::process(std::vector<Well>& isoho_wells) const
{
	for (auto& well : isoho_wells)
	{
		for (const auto& interval : well.intervals)
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

		const auto& zero_point = well.intervals[0];
		for (unsigned i = 1; i < well.intervals.size(); ++i)
		{
			auto& current_interval = well.intervals[i];
			auto& prev = well.intervals[i - 1];			
			
			const auto angle = current_interval.angle;
			const auto prev_angle = prev.angle;			

			const auto azimut = current_interval.azimut;
			const auto prev_azimut = prev.azimut;			

			const auto interval = current_interval.interval_end - prev.interval_end;
			
			current_interval.dz = prev.dz;
			current_interval.dx = prev.dx;
			if (isValidValue(azimut) && isValidValue(prev_azimut))
			{
				current_interval.dz += interval * mySin(prev_angle, angle) * mySin(prev_azimut, azimut);
				current_interval.dz -= zero_point.dz;

				current_interval.dx += interval * mySin(prev_angle, angle) * myCos(prev_azimut, azimut);
				current_interval.dx -= zero_point.dx;				

				well.intervals[i].curvature = calcKR(current_interval, prev);
			}

			current_interval.dy = prev.dy + interval * myCos(prev_angle, angle);
			current_interval.borehole = current_interval.interval_end - current_interval.dy + zero_point.dy;

			current_interval.dy -= zero_point.dy;
		}
	}
}

bool ARMDataProcessor::isValidValue(double a) const
{
	return abs(a - kNullValue) > kEpsilon;
}

double ARMDataProcessor::calcKR(const WellInterval& interval, const WellInterval& prev_interval) const
{
	if (isValidValue(interval.azimut) && isValidValue(prev_interval.azimut))
	{
		if (interval.interval_end - prev_interval.interval_end)
		{
			double value = 20.0f / (interval.interval_end - prev_interval.interval_end) * asin(
				sqrt(SinHD(interval.angle, prev_interval.angle) * SinHD(interval.angle, prev_interval.angle)
					+ SinHS(interval.angle, prev_interval.angle) * SinHS(interval.angle, prev_interval.angle)
					* SinHD(interval.azimut, prev_interval.azimut) * SinHD(interval.azimut, prev_interval.azimut)
				)) * kRad;

			return value;
		}
		else 
		{
			return {};
		}
	}
	return {};
}

double ARMDataProcessor::calcDX(const WellInterval& interval, const WellInterval & prev_interval, const WellInterval& zero_point) const
{
	return interval.dz - zero_point.dz;
}
