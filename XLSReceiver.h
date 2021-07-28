#pragma once
#include "Receiver.h"
#include <QAxObject>
#include "XLSWrapper.h"

class XLSReceiver :
    public Receiver
{
    QString filename_;
    struct ExcelObject
    {
        QAxObject* object;
        ExcelObject();
        ~ExcelObject();
    } excel;
    QAxObject* sheet;
public:
    XLSReceiver(const QString& filename);
    ~XLSReceiver();

    virtual std::vector<Well> getData() const override;

private:
    QAxObject* getSheet() const;
    std::optional<std::pair<size_t, size_t>> getDataRange() const;

    void fillWellNumber(Well & well) const;

    template <typename T>
    T getCellValue(int row, int col) const;
};

