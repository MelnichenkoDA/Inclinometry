#pragma once

#include "Uploader.h"

class ExcelUploader : public Uploader
{
	ExcelUploader();
	~ExcelUploader();
	virtual void uploadData(const std::vector<Well>& wells);
};

