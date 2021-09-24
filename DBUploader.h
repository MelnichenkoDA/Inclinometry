#pragma once
#include "Uploader.h"

class DBUploader :
    public Uploader
{
    QString klinz_link;
public:
    DBUploader();
    DBUploader(const QString & klinz);
    virtual ~DBUploader() override;
    virtual void uploadData(const std::vector<Well>&) override;


    QString wellToString(const Well& well) const;   
    QString intervalToString(const WellInterval& interval) const;
};

