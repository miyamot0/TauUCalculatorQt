
#include <QtMath>
#include "tauucalculations.h"
#include "tauumodel.h"

TauUCalculations::TauUCalculations() { }

void TauUCalculations::SetData(QList<double> baselines, QList<double> interventions, QString preStudy, QString preSubject, QString tag)
{
    BaselineObservations = baselines;
    TreatmentObservations = interventions;
    StudyName = preStudy;
    ParticipantName = preSubject;
}

TauUModel TauUCalculations::CompareTrend(bool correctBaseline, double confidence)
{
    int uU = 0,
        uL = 0,
        uT = 0,
        uPairs = 0,
        increment = 0;

    for (int i = 0; i < BaselineObservations.count() - 1; i++)
    {
        for (int j = 0; j < TreatmentObservations.count(); j++)
        {
            double diff = (TreatmentObservations[j] - BaselineObservations[i]);

            if (diff > 0)
            {
                uU++;
            }
            else if (diff < 0)
            {
                uL++;
            }
            else
            {
                uT++;
            }

            uPairs++;
        }

        increment++;
    }

    int S = uU - uL;

    double Vars = BaselineObservations.count() * TreatmentObservations.count() * (BaselineObservations.count() + TreatmentObservations.count() + 1) / 3.0;

    if (correctBaseline)
    {
        TauUModel mBl = BaselineTrend();
        S -= (int) mBl.S;
    }

    TauUModel mReturn;
    mReturn.S = S;
    mReturn.Pairs = uPairs;
    mReturn.Ties = uT;
    mReturn.TAU = (double)S / (double)uPairs;
    mReturn.TAUB = (S / (uPairs * 1.0 - uT * 0.5));
    mReturn.VARs = Vars;
    mReturn.SD = qSqrt(Vars);
    mReturn.SDtau = (qSqrt(Vars) / uPairs);
    mReturn.Z = ((S / (double)uPairs) / (qSqrt(Vars) / (double)uPairs));
    mReturn.PValue = GetPValueFromUDistribution(qFabs(((S / (double)uPairs) / (qSqrt(Vars) / (double)uPairs))));
    mReturn.CI << ((double)S / (double)uPairs - confidence * (qSqrt(Vars) / (double)uPairs)) << ((double)S / (double)uPairs + confidence * (qSqrt(Vars) / (double)uPairs));

    return mReturn;
}

TauUModel TauUCalculations::BaselineTrend()
{
    int uU = 0,
        uL = 0,
        uT = 0,
        uPairs = 0,
        increment = 0;

    for (int i = 0; i < BaselineObservations.count() - 1; i++)
    {
        for (int j = 1 + increment; j < TreatmentObservations.count(); j++)
        {
            double diff = (TreatmentObservations[j] - BaselineObservations[i]);

            if (diff > 0)
            {
                uU++;
            }
            else if (diff < 0)
            {
                uL++;
            }
            else
            {
                uT++;
            }

            uPairs++;
        }

        increment++;
    }

    int S = uU - uL;

    double Vars = BaselineObservations.count() * (BaselineObservations.count() - 1.0) * (2.0 * BaselineObservations.count() + 5.0) / 18.0;

    TauUModel mReturn;
    mReturn.S = S;
    mReturn.Pairs = uPairs;
    mReturn.Ties = uT;
    mReturn.TAU = (double)S / (double)uPairs;
    mReturn.TAUB = (S / (uPairs * 1.0 - uT * 0.5));
    mReturn.VARs = Vars;
    mReturn.SD = qSqrt(Vars);
    mReturn.SDtau = (qSqrt(Vars) / uPairs);
    mReturn.Z = ((S / (double)uPairs) / (qSqrt(Vars) / (double)uPairs));
    mReturn.PValue = GetPValueFromUDistribution(qFabs(((S / (double)uPairs) / (qSqrt(Vars) / (double)uPairs))));

    return mReturn;
}

double TauUCalculations::GetPValueFromUDistribution(double z)
{
    z = qFabs(z);
    double p2 = (((((.000005383*z+.0000488906)*z+.0000380036)*z+.0032776263)*z+.0211410061)*z+.049867347)*z+1;
    return qPow(p2, -16);
}
