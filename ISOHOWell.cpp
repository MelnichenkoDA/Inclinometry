#include "ISOHOWell.h"

Well::Well()
{}

Well::Well(size_t size)
	: interval_end_(size),
	interval_begin_(size),
	tilt_angle_(size),
	azimut_(size),
	borehole_(size),
	abs_depth_(size),
	dx_(size),
	dy_(size),
	borehole_shift_(size),
	directional_angle_(size),
	crooked_ness_(size),
	vertical_depth_(size)
{
}

Well::Well(const Well& other)
	: wellid_{ other.wellid_ },
	isMinute{ other.isMinute },
	magnite_deviation{ other.magnite_deviation },
	field_id_{ other.field_id_ },
	well_number_{ other.well_number_ },
	interval_end_{ other.interval_end_ },
	interval_begin_{other.interval_begin_},
	tilt_angle_{ other.tilt_angle_ },
	azimut_{ other.azimut_ },
	borehole_{ other.borehole_ },
	abs_depth_{ other.abs_depth_ },
	dx_{ other.dx_ },
	dy_{ other.dy_ },
	borehole_shift_{ other.borehole_shift_ },
	directional_angle_{ other.directional_angle_ },
	crooked_ness_{ other.crooked_ness_ },
	correction_time_{ other.correction_time_ },
	vertical_depth_{ other.vertical_depth_ }
{}
