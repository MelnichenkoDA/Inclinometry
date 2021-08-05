#include "XLSReceiver.h"

XLSReceiver::XLSReceiver(const QString& filename)
	:filename_(filename),
	excel_object{filename}
{}

XLSReceiver::~XLSReceiver()
{}

std::vector<Well> XLSReceiver::getData() const
{
	auto rows_count = excel_object.getRowsCount();
	//auto columns = excel_object.getColumnsCount();
	
	std::vector<Well> wells;
	wells.push_back(Well(rows_count - 1));
	auto& well = wells.back();
	fillWellNumber(well);
	
	for (size_t row = 2; row < rows_count + 1; ++row)
	{
		//GL_1, UG_1, AZ_1
		well.intervals[row - 2].interval_end = excel_object.getCellValue<double>(row, 5);
		if (row > 2)
			well.intervals[row - 2].interval_begin = well.intervals[row - 3].interval_end;
		well.intervals[row - 2].angle = excel_object.getCellValue<double>(row, 6);
		well.intervals[row - 2].azimut = excel_object.getCellValue<double>(row, 7);
	}
	return wells;
}

void XLSReceiver::fillWellNumber(Well& well) const
{
	well.ois_code = excel_object.getCellValue<double>(2, 2);
	well.field_id = excel_object.getCellValue<QString>(2, 3);
	well.well_number = excel_object.getCellValue<QString>(2, 4);
}
