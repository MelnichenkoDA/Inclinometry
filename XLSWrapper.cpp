#include "XLSWrapper.h"
#include <QFile>

XLSWrapper::XLSWrapper()
	:excel_object{},
	workbooks{},
	workbook{},
	sheets{},
	sheet{}
{}

XLSWrapper::XLSWrapper(const QString& filename)
	:excel_object{},
	workbooks{},
	workbook{},
	sheets{},
	sheet{}
{
	open(filename);
}

XLSWrapper::~XLSWrapper()
{
	close();
}

void XLSWrapper::open(const QString& filename)
{
	if (isOpen())
		close();

	excel_object = new QAxObject("Excel.Application");
	if (!excel_object) throw std::runtime_error("Couldn't create Excel Application oject");

	excel_object->setProperty("Visible", false);
	workbooks = excel_object->querySubObject("Workbooks");
	if (!workbooks) throw std::runtime_error("Couldn't get Workbooks from Excel object");

	if (QFile::exists(filename))
	{
		workbook = workbooks->querySubObject("Open(const QString&)", filename);
	}
	else
	{
		workbook = workbooks->querySubObject("Add");
	}
	if (!workbook) throw std::runtime_error("Couldn't get Workbook from Excel object");

	sheets = workbook->querySubObject("Sheets");
	if (!sheets) throw std::runtime_error("Couldn't get Worksheets from Excel object");

	sheet = sheets->querySubObject("Item(int)", 1);
	if (!sheet) throw std::runtime_error("Couldn't get Worksheet from Excel object");
}

bool XLSWrapper::isOpen() const
{
	if (excel_object && workbooks && workbook && sheets && sheet)
		return true;
	return false;
}

void XLSWrapper::saveAs(const QString& filename)
{
	QList<QVariant> lstParam;
	lstParam.append(filename);
	lstParam.append(-4143);
	lstParam.append("");
	lstParam.append("");
	lstParam.append(false);
	lstParam.append(false);
	lstParam.append(1);
	lstParam.append(2);
	lstParam.append(false);
	lstParam.append(false);
	lstParam.append(false);
	lstParam.append(false);
	workbook->dynamicCall("SaveAs(QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant)", lstParam);
}

int XLSWrapper::getRowsCount() const
{
	auto used_range = sheet->querySubObject("UsedRange");
	if (!used_range)
		throw std::runtime_error("Couldn't get rows count from Excel!");

	auto rows = used_range->querySubObject("Rows");
	if (!rows) throw std::runtime_error("Couldn't get rows count from Excel!");

	auto rows_count = rows->dynamicCall("Count()");
	if (rows_count.isNull())
		throw std::runtime_error("Couldn't get rows count from Excel!");

	return rows_count.toInt();
}

int XLSWrapper::getColumnsCount() const
{
	auto used_range = sheet->querySubObject("UsedRange");
	if (!used_range)
		throw std::runtime_error("Couldn't get columns count from Excel!");

	auto columns = used_range->querySubObject("Columns");
	if (!columns) throw std::runtime_error("Couldn't get columns count from Excel!");

	auto columns_count = columns->dynamicCall("Count()");
	if (columns_count.isNull())
		throw std::runtime_error("Couldn't get columns count from Excel!");

	return columns_count.toInt();
}

void XLSWrapper::close()
{
	delete sheet;
	sheet = {};

	delete sheets;
	sheets = {};

	workbook->dynamicCall("Close()");
	delete workbook;
	workbook = {};

	workbooks->dynamicCall("Close()", false);
	delete workbooks;
	workbooks = {};

	excel_object->dynamicCall("Quit(void)");
	delete excel_object;
	excel_object = {};
}
