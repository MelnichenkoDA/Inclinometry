#pragma once

#include "Uploader.h"
#include "XLSWrapper.h"

class ExcelUploader : public Uploader
{
public:
	ExcelUploader(const QString& filename);
	virtual ~ExcelUploader() override;
	virtual void uploadData(const std::vector<Well>& wells);

private:
	void uploadHeaders();
	void uploadWell(const Well& well, int & row);

	XLSWrapper excel_object;
	QString filename;
};

