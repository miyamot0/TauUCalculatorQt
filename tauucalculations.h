#ifndef TAUUCALCULATIONS_H
#define TAUUCALCULATIONS_H

#include <QObject>
#include "tauumodel.h"

class TauUCalculations
{
public:
    TauUCalculations();
    void SetData(QList<double> baselines, QList<double> interventions, QString preStudy, QString preSubject, QString tag);

    TauUModel BaselineTrend();
    TauUModel CompareTrend(bool correctBaseline, double confidence);
    double GetPValueFromUDistribution(double z);

private:
    QString StudyName;
    QString ParticipantName;
    QString PhaseName;

    QList<double> BaselineObservations;
    QList<double> TreatmentObservations;
};

#endif // TAUUCALCULATIONS_H
