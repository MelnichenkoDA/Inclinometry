#include "ExcelUploader.h"

namespace
{
	//int - column number, string - column title
	const std::map<QString, int> kColumnHeaders
	{
		{"SK", 1},		//Скважина
		{"MS", 2},		//Месторождение
		{"S1", 3},		//Геологический номер скважины
		{"GL", 4},		//Глубина конца интервала, м
		{"UG", 5},		//Угол, гpад
		{"AZ", 6},		//Азимут, гpад
		{"UL", 7},		//Удлинение, м
		{"ZZ", 8},		//Абсолютная отметка
		{"XX", 9},		//Координата Х
		{"YY", 10},		//Координата Y
		{"SM", 11},		//Смещение (по линии устье-забой), м
		{"DU", 12},		//Дирекционный угол, град
		{"KR", 13},		//Кривизна град / 10 м
		{"TK", 14},		//Дата коppектиpовки
		{"GLN", 15},		//Глубина начала интервала, м
		{"KR_MAX", 16},	//Признак максимальной кривизны
		{"ZB2", 17},		//Номер ствола
		{"GRAZ", 18},		//Географический азимут на глубине замера
		{"GLV", 19}		//Глубина по вертикали
	};
}

ExcelUploader::ExcelUploader(const QString& upload_filename)
	:excel_object{ upload_filename },
	filename{ upload_filename }
{}

ExcelUploader::~ExcelUploader()
{}

void ExcelUploader::uploadData(const std::vector<Well>& wells)
{
	uploadHeaders();
	int row = 2;
	for (auto& well : wells)
	{
		uploadWell(well, row);
	}

	excel_object.saveAs(filename);
}

void ExcelUploader::uploadHeaders()
{
	int row = 1;
	int col = 1;
	for (const auto& [title, col] : kColumnHeaders)
		excel_object.setCellValue(row, col, title);
}

void ExcelUploader::uploadWell(const Well& well, int & row)
{
	for (const auto& interval : well.intervals)
	{
		excel_object.setCellValue(row, kColumnHeaders.at("SK"), well.ois_code);
		excel_object.setCellValue(row, kColumnHeaders.at("MS"), well.field_id);
		excel_object.setCellValue(row, kColumnHeaders.at("S1"), well.well_number);
		excel_object.setCellValue(row, kColumnHeaders.at("GL"), interval.interval_end);
		excel_object.setCellValue(row, kColumnHeaders.at("UG"), interval.angle);
		excel_object.setCellValue(row, kColumnHeaders.at("AZ"), interval.azimut);
		excel_object.setCellValue(row, kColumnHeaders.at("UL"), interval.borehole);
		excel_object.setCellValue(row, kColumnHeaders.at("ZZ"), interval.dz);
		excel_object.setCellValue(row, kColumnHeaders.at("XX"), interval.dx);
		excel_object.setCellValue(row, kColumnHeaders.at("YY"), interval.dy);
		excel_object.setCellValue(row, kColumnHeaders.at("SM"), interval.displacement);
		excel_object.setCellValue(row, kColumnHeaders.at("DU"), interval.directional_angle);
		excel_object.setCellValue(row, kColumnHeaders.at("KR"), interval.curvature);
		excel_object.setCellValue(row, kColumnHeaders.at("TK"), well.correction_time);
		excel_object.setCellValue(row, kColumnHeaders.at("GLN"), interval.interval_begin);
		excel_object.setCellValue(row, kColumnHeaders.at("GLV"), interval.vertical_depth);
		++row;
	}
	
}
