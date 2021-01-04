#ifndef CHAMBRE_H
#define CHAMBRE_H

#include <QString>
class RoomStmModel
{
public:
    RoomStmModel(QString nom, double temp, double pressure);

    double temp() const;
    void setTemp(double temp);

    double pressure() const;
    void setPressure(double pressure);

    QString name() const;
    void setName(const QString &name);

private:
    double mTemp;
    double mPressure;
    QString mName;
};

#endif // CHAMBRE_H
