#pragma once
#include <QAxObject>

class XLSWrapper
{
public:
	XLSWrapper();
	XLSWrapper(const QString& filename);
	~XLSWrapper();

	void open(const QString& filename);
	bool isOpen();

	void saveAs(const QString& filename);

	int getRowsCount();
	int getColumnsCount();

	template<class T>
	T getCellValue(int row, int col);
	
	template<class T>
	void setCellValue(int row, int col, const T& value);

private:
	void close();

	QAxObject* excel_object;
	QAxObject* workbooks;
	QAxObject* workbook;
	QAxObject* sheets;
	QAxObject* sheet;
};

template<class T>
inline T XLSWrapper::getCellValue(int row, int col)
{
	auto cell = sheet->querySubObject("Cells(int, int)", row, col);
	if (!cell) throw std::runtime_error("Couldn't get cell(" + std::to_string(row) + "," + std::to_string(col) + ")");

	auto value = cell->dynamicCall("Value()");
	if (value.isNull())
		throw std::runtime_error("Couldn't get value from cell(" + std::to_string(row) + "," + std::to_string(col) + ")");
	return value.value<T>();
}

template<class T>
inline void XLSWrapper::setCellValue(int row, int col, const T& value)
{
	auto cell = sheet->querySubObject("Cells(int, int)", row, col);
	if (!cell) throw std::runtime_error("Couldn't get cell(" + std::to_string(row) + "," + std::to_string(col) + ")");

	cell->setProperty("Value", QVariant::fromValue(value));
}
