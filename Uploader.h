#pragma once
#include "ISOHOWell.h"

class Uploader
{
	Uploader() {}
	virtual ~Uploader() {}
	virtual void uploadData(const std::vector<Well>&) = 0;
};

