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
