/**
   Copyright 2017 Shawn Gilroy

   This file is part of Tau-U Calculator, Qt port.

   Tau-U Calculator is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3.

   Tau-U Calculator is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Tau-U Calculator.  If not, see http://www.gnu.org/licenses/.

   The Tau-U Calculator is a tool to assist researchers in behavior economics.

   Email: shawn(dot)gilroy(at)temple.edu

  */

#ifndef TAUUMODEL_H
#define TAUUMODEL_H

#include <QObject>

class TauUModel
{
public:
    TauUModel();

    QString StudyName;
    QString ParticipantName;
    int S;
    int Pairs;
    int Ties;
    bool IsCorrected;
    double TAU;
    double TAUB;
    double VARs;
    double SD;
    double SDtau;
    double Z;
    double PValue;
    QList<double> CI;

};


/*
    omniTau.StudyName = studyName;
    omniTau.S = S;
    omniTau.Pairs = pairs;
    omniTau.Ties = ties;
    omniTau.IsCorrected = isCorrected;
    omniTau.TAU = totalTauES / totalInverseVariance;
    omniTau.TAUB = (S / (pairs * 1.0 - ties * 0.5));
    omniTau.VARs = (1.0 / totalInverseVariance);
    omniTau.SD = qSqrt(omniTau.VARs);
    omniTau.SDtau = qSqrt(1.0 / totalInverseVariance);
    omniTau.Z = z;
    omniTau.PValue = pval;
    omniTau.CI << ((totalTauES / totalInverseVariance) - confidence * globalSETau) << ((totalTauES / totalInverseVariance) + confidence * globalSETau);
*/

#endif // TAUUMODEL_H
