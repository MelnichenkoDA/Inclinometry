#include "DBUploader.h"

namespace
{
	const QString kInsertTitle
	{
		R"raw(
insert into %1 
(SK_1,MS_1,S1_1,GL_1,UG_1,AZ_1,UL_1,ZZ_1,XX_1,YY_1,SM_1,DU_1,KR_1,TK_1,GLN_1,KR_MAX_1,ZB2_1,GRAZ_1,GLV_1,DKL_1) 	
select 
		)raw"
	};

	const QString kFromDual
	{
		R"raw(
from dual 
		)raw"
	};

	const QString kFromUnion
	{
		R"raw( 
from dual 
union all select 
		)raw"
	};
}

DBUploader::DBUploader()
{
}

DBUploader::DBUploader(const QString& klinz)
	: klinz_link{ klinz }
{}

DBUploader::~DBUploader()
{}

void DBUploader::uploadData(const std::vector<Well>& wells)
{
	QString query = kInsertTitle.arg("GGD_PERMBS.klinz");
	for (const auto& well : wells)
	{
		auto temp = wellToString(well);
		query += temp;
	}
}

QString DBUploader::wellToString(const Well& well) const
{
	QString well_info = well.ois_code + ", ";
	well_info += "'" + well.field_id + "', ";
	well_info += "'" + well.well_number + "', ";

	QString interval_info, result;
	for (int i = 0; i < well.intervals.size(); ++i)
	{
		QString interval_info;
		const auto& interval = well.intervals[i];		

		interval_info += QString::number(interval.interval_end, 'f', 2) + ", ";
		interval_info += QString::number(interval.angle, 'f', 2) + ", ";
		interval_info += QString::number(interval.azimut, 'f', 2) + ", ";

		interval_info += QString::number(interval.borehole, 'f', 2) + ", ";
		interval_info += QString::number(interval.dz, 'f', 2) + ", ";
		interval_info += QString::number(interval.dx, 'f', 2) + ", ";

		interval_info += QString::number(interval.dy, 'f', 2) + ", ";
		interval_info += QString::number(interval.displacement, 'f', 2) + ", ";
		interval_info += QString::number(interval.directional_angle, 'f', 2) + ", ";

		interval_info += QString::number(interval.curvature, 'f', 2) + ", ";
		interval_info += "'" + well.correction_time + "', ";
		interval_info += QString::number(interval.interval_begin, 'f', 2) + ", ";

		interval_info += "'" + interval.curvature_max + "', ";
		interval_info += QString::number(interval.inclinometry, 'f', 2) + ", ";
		interval_info += QString::number(interval.geographic_azimut, 'f', 2) + ", ";

		interval_info += QString::number(interval.vertical_depth, 'f', 2) + ", ";
		interval_info += "to_date('" + interval.measurement_date.toString(Qt::ISODate) + "', 'yyyy-mm-dd')";
		
		result += well_info + interval_info;
		if (i != well.intervals.size() - 1)
		{
			result += kFromUnion;
		}
		else
		{
			result += kFromDual;
		}
	}
	
	return result;
}

QString DBUploader::intervalToString(const WellInterval& interval) const
{
	return QString{};
}

