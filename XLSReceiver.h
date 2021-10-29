#pragma once
#include "Receiver.h"
#include "XLSWrapper.h"

class XLSReceiver :
    public Receiver
{
    QString filename_;
    XLSWrapper excel_object;
public:
    XLSReceiver(const QString& filename);
    ~XLSReceiver();

    virtual std::vector<Well> getData() const override;

private:
    void fillWellNumber(Well & well) const;
};

