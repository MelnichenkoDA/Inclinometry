#include "ISOHOWell.h"

Well::Well()
	: isMinute{ true },
	ois_code{},
	magnite_deviation{},
	field_id{},
	well_number{},
	correction_time{},
	intervals{}
{}

Well::Well(size_t size)
	: isMinute{ true },
	ois_code{},
	magnite_deviation{},
	field_id{},
	well_number{},
	correction_time{},
	intervals{ size }
{}

Well::Well(const Well& other)
	: isMinute{ other.isMinute },
	ois_code{ other.ois_code },
	magnite_deviation{ other.magnite_deviation },
	field_id{ other.field_id },
	well_number{ other.well_number },
	correction_time{ other.correction_time },
	intervals{ other.intervals }
{}