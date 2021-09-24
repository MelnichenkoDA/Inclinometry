#include "WellInterval.h"
#include <QSqlQuery>
#include <QVariant>

WellInterval::WellInterval()
	: interval_end{},
	interval_begin{},
	angle{},
	azimut{},
	borehole{},
	dx{},
	dy{},
	dz{},
	displacement{},
	directional_angle{},
	curvature{},
	vertical_depth{},
	curvature_max{},
	inclinometry{},
	geographic_azimut{},
	measurement_date{2000, 1, 1}
{}


WellInterval::WellInterval(const QSqlQuery& query)
	:interval_end{query.value(3).toDouble()},
	interval_begin{},
	angle{query.value(4).toDouble()},
	azimut{query.value(5).toDouble()},
	borehole{query.value(6).toDouble()},
	dx{query.value(8).toDouble()},
	dy{query.value(9).toDouble()},
	dz{ query.value(7).toDouble() },
	displacement{query.value(10).toDouble()},
	directional_angle{query.value(11).toDouble()},
	curvature{query.value(12).toDouble()},
	vertical_depth{query.value(18).toDouble()},
	curvature_max{query.value(15).toString()},
	inclinometry{query.value(16).toDouble()},
	geographic_azimut{query.value(17).toDouble()},
	measurement_date{query.value(19).toDate()}
{}

WellInterval::WellInterval(const WellInterval& other)
	: interval_end{ other.interval_end },
	interval_begin{ other.interval_begin },
	angle{ other.angle },
	azimut{ other.azimut },
	borehole{ other.borehole },
	dx{ other.dx },
	dy{ other.dy },
	dz{ other.dz },
	displacement{ other.displacement },
	directional_angle{ other.directional_angle },
	curvature{ other.curvature },
	vertical_depth{ other.vertical_depth },
	curvature_max{ other.curvature_max },
	inclinometry{ other.inclinometry },
	geographic_azimut{ other.geographic_azimut },
	measurement_date{ other.measurement_date }
{}