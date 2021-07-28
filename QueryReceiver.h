#pragma once
#include "Receiver.h"
#include "DBConnection.h"

class QueryReceiver :
    public Receiver
{
public:
    QueryReceiver();

    virtual std::vector<Well> getData() const override;

private:
    QStringList getConnectionData() const;
    QString getOrganization() const;
    std::vector<std::pair<QString, QString>> getWellUWI(const DBConnection & connection, const QString & organization_id) const;
    std::vector<std::pair<QString, QString>> getWells() const;

    std::vector<Well> getInclinometries(const QString& uwi, const QString& srvy) const;
    void initWell(Well& well, const QSqlQuery& query);

    DBConnection isoho_connection;
    DBConnection plus_connection;
};

