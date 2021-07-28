#include "WellInterval.h"
#include <QVariant>

WellInterval::WellInterval()
	: interval_end{},
	interval_begin{},
	angle{},
	azimut{},
	borehole{},
	abs_depth{},
	dx{},
	dy{},
	displacement{},
	directional_angle{},
	curvature{},
	vertical_depth{}
{}


WellInterval::WellInterval(const QSqlQuery& query)
	:interval_end{query.value(3).toDouble()},
	interval_begin{},
	angle{query.value(4).toDouble()},
	azimut{query.value(5).toDouble()},
	borehole{query.value(6).toDouble()},
	abs_depth{query.value(7).toDouble()},
	dx{query.value(8).toDouble()},
	dy{query.value(9).toDouble()},
	displacement{query.value(10).toDouble()},
	directional_angle{query.value(11).toDouble()},
	curvature{query.value(12).toDouble()},
	vertical_depth{query.value(17).toDouble()}
{}

WellInterval::WellInterval(const WellInterval& other)
	: interval_end{ other.interval_end },
	interval_begin{ other.interval_begin },
	angle{ other.angle },
	azimut{ other.azimut },
	borehole{ other.borehole },
	abs_depth{ other.abs_depth },
	dx{ other.dx },
	dy{ other.dy },
	displacement{ other.displacement },
	directional_angle{ other.directional_angle },
	curvature{ other.curvature },
	vertical_depth{ other.vertical_depth }
{}