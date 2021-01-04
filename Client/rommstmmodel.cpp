#include "rommstmmodel.h"

RoomStmModel::RoomStmModel(QString nom, double temp, double pressure)
{
    mName = nom;
    mTemp = temp;
    mPressure = pressure;
}

double RoomStmModel::temp() const
{
    return mTemp;
}

void RoomStmModel::setTemp(double temp)
{
    mTemp = temp;
}

double RoomStmModel::pressure() const
{
    return mPressure;
}

void RoomStmModel::setPressure(double pressure)
{
    mPressure = pressure;
}

QString RoomStmModel::name() const
{
    return mName;
}

void RoomStmModel::setName(const QString &name)
{
    mName = name;
}
