#include "XLSReceiver.h"

XLSReceiver::XLSReceiver(const QString& filename)
	:filename_(filename),
	excel{},
	sheet{getSheet()}
{}

XLSReceiver::~XLSReceiver()
{}

std::vector<Well> XLSReceiver::getData() const
{
	size_t rows_count{}, columns_count{};
	if (const auto range = getDataRange(); range.has_value())
	{
		rows_count = range.value().first;
		columns_count = range.value().second;
	}
	else
	{
		throw std::runtime_error("Couldn't get data range from Excel file");
	}
	
	std::vector<Well> wells;
	wells.push_back(Well(rows_count - 1));
	auto& well = wells.back();
	fillWellNumber(well);
	
	for (size_t row = 2; row < rows_count + 1; ++row)
	{
		//GL_1, UG_1, AZ_1
		well.intervals[row - 2].interval_end = getCellValue<double>(row, 5);
		if (row > 2)
			well.intervals[row - 2].interval_begin = well.intervals[row - 3].interval_end;
		well.intervals[row - 2].angle = getCellValue<double>(row, 6);
		well.intervals[row - 2].azimut = getCellValue<double>(row, 7);
	}
	return wells;
}

QAxObject* XLSReceiver::getSheet() const
{
	auto workbooks = excel.object->querySubObject("Workbooks");
	if (!workbooks) throw std::runtime_error("Couldn't get Workbooks from Excel object");

	auto workbook = workbooks->querySubObject("Open(const QString&)", filename_);
	if (!workbook) throw std::runtime_error("Couldn't get Workbook from Excel object");

	auto sheets = workbook->querySubObject("Worksheets");
	if (!sheets) throw std::runtime_error("Couldn't get Worksheets from Excel object");

	auto sheet = sheets->querySubObject("Item(int)", 1);
	if (!sheet) throw std::runtime_error("Couldn't get Worksheet from Excel object");

	return sheet;
}

std::optional<std::pair<size_t, size_t>> XLSReceiver::getDataRange() const
{
	auto* range = sheet->querySubObject("UsedRange");
	if (!range)
		return {};

	auto* columns = range->querySubObject("Columns");
	if (!columns)
		return {};
	auto columns_count = columns->dynamicCall("Count()").toInt();

	auto* rows = range->querySubObject("Rows");
	if (!rows)
		return {};
	auto rows_count = rows->dynamicCall("Count()").toInt();

	return { std::pair{rows_count, columns_count} };
}

void XLSReceiver::fillWellNumber(Well& well) const
{
	well.ois_code = getCellValue<double>(2, 2);
	well.field_id = getCellValue<QString>(2, 3);
	well.well_number = getCellValue<QString>(2, 4);
}

template <typename T>
T XLSReceiver::getCellValue(int row, int col) const
{
	auto cell = sheet->querySubObject("Cells(int, int)", row, col);
	if (!cell) throw std::runtime_error("Couldn't get cell with row = " + std::to_string(row) + " and col = " + std::to_string(col));

	auto value = cell->dynamicCall("Value()");
	return value.value<T>();
}

XLSReceiver::ExcelObject::ExcelObject()
{
	object = new QAxObject("Excel.Application");
	if (!object) throw std::runtime_error("Couldn't create Excel Application oject");
}

XLSReceiver::ExcelObject::~ExcelObject()
{
	object->dynamicCall("Quit()");
}
