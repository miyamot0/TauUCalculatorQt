/**
   Copyright 2017 Shawn Gilroy

   This file is part of Discounting Model Selector, Qt port.

   Discounting Model Selector is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3.

   Discounting Model Selector is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Discounting Model Selector.  If not, see http://www.gnu.org/licenses/.

   The Discounting Model Selector is a tool to assist researchers in behavior economics.

   Email: shawn(dot)gilroy(at)temple.edu

  */

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
    int pos = 0;
    int neg = 0;
    int ties = 0;
    int pairs = 0;

    bool istrend = false;
    int trend_off = 0;
    int inc = 0;

    for (int i = 0; i < BaselineObservations.count() - trend_off; i++)
    {
        for (int j = trend_off + inc; j < TreatmentObservations.count(); j++)
        {
            double diff = (TreatmentObservations[j] - BaselineObservations[i]);

            if (diff == 0 || (QString::number(TreatmentObservations[j]) == QString::number(BaselineObservations[i])))
            {
                /* Hackish workaround for odd floating point comparisons */
                ties++;
            }
            else if (diff > 0)
            {
                pos++;
            }
            else
            {
                neg++;
            }

            pairs++;
        }

        if (istrend)
        {
            inc++;
        }
    }

    int S = pos - neg;
    double Vars = BaselineObservations.count() * TreatmentObservations.count() * (BaselineObservations.count() + TreatmentObservations.count() + 1) / 3.0;

    if (correctBaseline)
    {
        TauUModel mBl = BaselineTrend();
        S -= (int) mBl.S;
    }

    TauUModel mReturn;
    mReturn.StudyName = StudyName;
    mReturn.ParticipantName = ParticipantName;
    mReturn.S = S;
    mReturn.Pairs = pairs;
    mReturn.Ties = ties;
    mReturn.TAU = (double)S / (double)pairs;
    mReturn.TAUB = (S / (pairs * 1.0 - ties * 0.5));
    mReturn.VARs = Vars;
    mReturn.SD = qSqrt(Vars);
    mReturn.SDtau = (qSqrt(Vars) / pairs);
    mReturn.Z = ((S / (double)pairs) / (qSqrt(Vars) / (double)pairs));
    mReturn.PValue = GetPValueFromUDistribution(qFabs(((S / (double)pairs) / (qSqrt(Vars) / (double)pairs))));
    mReturn.CI << ((double)S / (double)pairs - confidence * (qSqrt(Vars) / (double)pairs)) << ((double)S / (double)pairs + confidence * (qSqrt(Vars) / (double)pairs));

    return mReturn;
}

TauUModel TauUCalculations::BaselineTrend()
{
    int pos = 0, neg = 0, ties = 0, pairs = 0;
    int inc = 0;
    int trend_off = 1;

    for (int i = 0; i < BaselineObservations.count() - trend_off; i++)
    {
        for (int j = trend_off + inc; j < BaselineObservations.count(); j++)
        {
            double diff = (BaselineObservations[j] - BaselineObservations[i]);

            if (diff == 0 || (QString::number(BaselineObservations[j]) == QString::number(BaselineObservations[i])))
            {
                /* Hackish workaround for odd floating point comparisons */
                ties++;
            }
            else if (diff > 0)
            {
                pos++;
            }
            else
            {
                neg++;
            }

            pairs++;
        }

        inc++;
    }

    int S = pos - neg;

    /* Variance as Defined by two-tailed Mann-Kendall test */
    double Vars = BaselineObservations.count() * (BaselineObservations.count() - 1.0) * (2.0 * BaselineObservations.count() + 5.0) / 18.0;

    TauUModel mReturn;
    mReturn.S = S;
    mReturn.Pairs = pairs;
    mReturn.Ties = ties;
    mReturn.TAU = (double)S / (double)pairs;
    mReturn.TAUB = (S / (pairs * 1.0 - ties * 0.5));
    mReturn.VARs = Vars;
    mReturn.SD = qSqrt(Vars);
    mReturn.SDtau = (qSqrt(Vars) / pairs);
    mReturn.Z = ((S / (double)pairs) / (qSqrt(Vars) / (double)pairs));
    mReturn.PValue = GetPValueFromUDistribution(qFabs(((S / (double)pairs) / (qSqrt(Vars) / (double)pairs))));

    return mReturn;
}

double TauUCalculations::GetPValueFromUDistribution(double z)
{
    z = qFabs(z);
    double p2 = (((((.000005383*z+.0000488906)*z+.0000380036)*z+.0032776263)*z+.0211410061)*z+.049867347)*z+1;
    return qPow(p2, -16);
}
