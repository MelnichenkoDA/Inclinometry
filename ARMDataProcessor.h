#pragma once
#include "ISOHOWell.h"

class ARMDataProcessor
{
	const double grad = 0.017453292519943295769236907684886f;
public:
	ARMDataProcessor();

	void process(std::vector<Well>& isoho_wells) const;

private:
	double mySin(double a, double b) const;
	double myCos(double a, double b) const;
	bool isValidValue(double a) const;
};

