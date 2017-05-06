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
