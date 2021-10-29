#include "QueryReceiver.h"

#include <QVariant>
#include <QDir>
#include <stdexcept>

namespace
{
	const QString kGetWellsList
	{
		R"raw(
			select distinct wh.uwi, p.dir_srvy_id, wh.operator, ba1.assoc_name
			from ggd_rtk.well_dir_srvy_pts p
			left join ggd_rtk.well_hdr wh on p.uwi=wh.uwi
			left join pr_ggd_rtk.well_hdr_corr whc on whc.nsi_id=wh.corp_id      
			left join ggd_rtk.well_dir_srvy_hdr h on h.uwi=wh.uwi and h.SURVEY_MODE='IR'
			left join codes_lukoil.business_assoc ba1 on ba1.assoc_id=wh.operator       
			where wh.operator=%1
		)raw"
	};

	const QString kGetWellInclinometry
	{
		R"raw(
			select distinct
            whc.ois_code as                               SK_1          --1 Скважина (уникальный идентификатор)     Not Null
            ,'MS'||trim(to_char(fhc.local_id,'0099')) as  MS_1          --2 Месторождение    
            ,ltrim(replace(wh.well_name,'_',''),'0') as   S1_1          --3 Геологический номер скважины    Char (7) 
            ,md as                                        GL_1          --4 Глубина конца интервала, м    Number (6,2)  PK Not Null
            ,deviation_angle as                           UG_1          --5 Угол, гpад          Number (5,2)  
            ,magnetic_azimuth as                          AZ_1          --6 Азимут, гpад        Number (5,2)  
            ,borehole_ext as                              UL_1          --7 Удлинение, м    Number (6,2)  
            ,p.tvd as                                     ZZ_1          --8 Абсолютная отметка    Number (5,1)  
            ,dx as                                        XX_1          --9 Координата Х (инклинометрия)    Number (7,1)  
            ,dy as                                        YY_1          --10  Координата Y (инклинометрия)    Number (7,1)  
            ,borehole_shift as                            SM_1          --11  Смещение (по линии устье-забой), м    Number (5,1)  
            ,DOG_LEG_SEVERITY  as                         DU_1          --12  Дирекционный угол, град   Number (6,2)  
            ,crookedness as                               KR_1          --13  Кривизна град / 10 м    Number (4,2)  
            ,to_char(p.LAST_UPDATE,'YYYYMMDDHH24MI') as   TK_1          --14  Дата коppектиpовки        VarChar2 (12) 
            ,'NO0000' as                                  KR_MAX_1      --15  Признак максимальной кривизны NO  Char (6)  Default 'NO0000'
            ,0 as                                         ZB2_1         --16  Номер ствола (инклинометрия)    Number (6)  PK Not Null Default 0
            ,0 as                                         GRAZ_1        --17  Географический азимут на глубине замера     Number  Default 0
            ,'' as                                        GLV_1         --18  Глубина по вертикали    Number  
            ,h.survey_date as							  dkl_1         --19
            ,wh.uwi                                                     --20
          from ggd_rtk.well_dir_srvy_pts p                              -- записи замера
            left join ggd_rtk.well_hdr wh on p.uwi=wh.uwi               -- заголовок скважины
            left join ggd_rtk.well_dir_srvy_hdr h on h.uwi=wh.uwi       -- заголовок замера
            left join pr_ggd_rtk.well_hdr_corr whc on whc.nsi_id=wh.corp_id -- справочник скважин
            left join pr_ggd_rtk.field_hdr_corr fhc on fhc.corp_id=wh.field  --справочник месторождений
            left join codes_lukoil.business_assoc ba on ba.assoc_id=wh.company_division
          where   h.SURVEY_MODE='IR'
            and wh.uwi in (%1) and p.dir_srvy_id in (%2)
			and h.survey_date=(select max(survey_date) from ggd_rtk.well_dir_srvy_hdr h1 where h1.uwi=wh.uwi and h1.dir_srvy_id=p.dir_srvy_id)      
          order by wh.uwi,md
		)raw"
	};

	const QString kInsertWellsValues
	{
		R"raw(
            insert into klinz@[дблинк]
            (SK_1,MS_1,S1_1,GL_1,UG_1,AZ_1,UL_1,ZZ_1,XX_1,YY_1,SM_1,DU_1,KR_1,TK_1,GLN_1,KR_MAX_1,ZB2_1,GRAZ_1,GLV_1,DKL_1)
            from
                select SK_1,MS_1,S1_1,GL_1,UG_1,AZ_1,UL_1,ZZ_1,XX_1,YY_1,SM_1,DU_1,KR_1,TK_1,GLN_1,KR_MAX_1,ZB2_1,GRAZ_1,GLV_1,DKL_1 
                from dual
            union all 
                select SK_1,MS_1,S1_1,GL_1,UG_1,AZ_1,UL_1,ZZ_1,XX_1,YY_1,SM_1,DU_1,KR_1,TK_1,GLN_1,KR_MAX_1,ZB2_1,GRAZ_1,GLV_1,DKL_1 from dual
        )raw"
	};

	const QString kIsohoConfig = "source.txt";
}

QueryReceiver::QueryReceiver()
	:isoho_connection{ kIsohoConfig, "isoho_connection"},
	plus_connection{"plus_connection"}
{}

std::vector<Well> QueryReceiver::getData() const
{
	if (!isoho_connection.isOpen())
		throw std::runtime_error(isoho_connection.getLastError().text().toStdString());

	const auto isoho_wells = getWells();
	QString uwi_id, srvy_id;
	for (const auto itr = isoho_wells.cbegin(); itr != std::prev(isoho_wells.cend());)
	{
		uwi_id += itr->first + ", ";
		srvy_id += itr->second + ", ";
	}
	uwi_id += isoho_wells.back().first;
	srvy_id += isoho_wells.back().second;

	return getInclinometries(uwi_id, srvy_id);
}

QString QueryReceiver::getOrganization() const
{
	return QString();
}

std::vector<std::pair<QString, QString>> QueryReceiver::getWellUWI(const DBConnection& connection, const QString& organization_id) const
{
	QSqlQuery query(connection.getDatabase());
	if (query.exec(kGetWellsList.arg(organization_id)))
	{
		std::vector<std::pair<QString, QString>> wells_list(query.size());
		size_t ind{};
		while (query.next())
			wells_list[ind++] = std::pair<QString, QString>(query.value(0).toString(), query.value(1).toString());
		return wells_list;
	}
	else
	{
		throw std::runtime_error(query.lastError().text().toStdString());
	}
	return {};
}

std::vector<std::pair<QString, QString>> QueryReceiver::getWells() const
{
	auto isoho_wells = getWellUWI(isoho_connection, "TEMP");
	auto plus_wells = getWellUWI(plus_connection, "TEMP");
	for (auto itr = isoho_wells.begin(); itr != isoho_wells.end();)
		if (std::find_if(plus_wells.cbegin(), plus_wells.cend(),
			[itr](const std::pair<QString, QString>& other) {return itr->first == other.first && itr->second == other.second; })
			!= plus_wells.cend())
			itr = isoho_wells.erase(itr);
		else
			++itr;

	return isoho_wells;
}

std::vector<Well> QueryReceiver::getInclinometries(const QString& uwi, const QString& srvy) const
{
	QSqlQuery query(isoho_connection.getDatabase());
	std::vector<Well> wells_data;
	if (query.exec(kGetWellInclinometry.arg(uwi).arg(srvy)))
	{		
		Well* prev{};
		while (query.next())
		{
			if (!prev || (prev && prev->ois_code != query.value(0)))
			{
				wells_data.emplace_back(Well{});
				prev = &wells_data.back();

				prev->ois_code = query.value(0).toDouble();
				prev->field_id = query.value(1).toDouble();
				prev->well_number = query.value(2).toString();
				prev->correction_time = query.value(13).toString();
				prev->uwi = query.value(19).toString();
				//prev->magnite_deviation = query.value().toDouble();
			}
			prev->intervals.emplace_back(WellInterval{ query });
			if (prev->intervals.size() != 1)
			{
				auto& last_interval = prev->intervals.back();
				auto& prev_interval = prev->intervals[prev->intervals.size() - 1];
				last_interval.interval_end = prev_interval.interval_begin;
			}
		}
	}
	else
	{
		throw std::runtime_error(query.lastError().text().toStdString());
	}

	return wells_data;
}

void QueryReceiver::initWell(Well& well, const QSqlQuery& query)
{
	well.ois_code = query.value(0).toDouble();
	well.field_id = query.value(1).toDouble();
	well.well_number = query.value(2).toString();

}
