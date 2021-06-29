#pragma once
#include <qobject.h>
#include <memory>

#include "ISOHOWell.h"

class Receiver : public
	QObject
{
public:
	Receiver()
		:QObject()
	{}

	virtual ~Receiver() = default;

	virtual std::vector<Well> getData() const = 0;
};

