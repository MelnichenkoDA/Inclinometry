#pragma once
#include "ISOHOWell.h"

class Uploader
{
public:
	Uploader() {}
	virtual ~Uploader() {}
	virtual void uploadData(const std::vector<Well>&) = 0;
};

