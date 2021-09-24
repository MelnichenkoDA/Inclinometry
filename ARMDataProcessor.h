#pragma once
#include "ISOHOWell.h"

class ARMDataProcessor
{
public:
	ARMDataProcessor();

	void process(std::vector<Well>& isoho_wells) const;

private:
	bool isValidValue(double a) const;
	double calcKR(const WellInterval & interval, const WellInterval& prev_interval) const;
	double calcDX(const WellInterval & interval, const WellInterval & prev_interval, const WellInterval& zero_point) const;
};

