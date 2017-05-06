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

   The SheetWidget class was inspired by the Qt 5.8 Spreadsheet example, its license is below:

   =======================================================================================================

   Copyright (C) 2016 The Qt Company Ltd.
   Contact: https://www.qt.io/licensing/

   This file is part of the demonstration applications of the Qt Toolkit.

   Commercial License Usage
   Licensees holding valid commercial Qt licenses may use this file in
   accordance with the commercial license agreement provided with the
   Software or, alternatively, in accordance with the terms contained in
   a written agreement between you and The Qt Company. For licensing terms
   and conditions see https://www.qt.io/terms-conditions. For further
   information use the contact form at https://www.qt.io/contact-us.

   BSD License Usage
   Alternatively, you may use this file under the terms of the BSD license
   as follows:

   "Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in
       the documentation and/or other materials provided with the
       distribution.
     * Neither the name of The Qt Company Ltd nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without specific prior written permission.


   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."

  */

#ifndef SHEETWIDGET_H
#define SHEETWIDGET_H

#include <QMainWindow>
#include <QProcess>
#include <QWidget>
#include <QTableWidget>
#include <QtGui>
#include <QtCharts>

#include "sheetselectdialog.h"
#include "resultsdialog.h"

#include "licensedialog.h"
#include "creditsdialog.h"
#include "aboutdialog.h"

#include "taudialog.h"
#include "tauumodel.h"
#include "tauucalculations.h"

class SheetWidget : public QMainWindow
{
    Q_OBJECT

public:
    SheetWidget(QWidget *parent = 0);

    void convertExcelColumn(QString &mString, int column);
    void ParseData(bool correct, double threshold, double confidence, QString figures);
    TauUModel CombineTaus(QList<TauUModel> selectedTaus, double confidence, QString studyName);

    QString convert_bool(bool value);

    QList<QStringList> allResults;
    QList<QStringList> allCharts;

    QList<TauUModel> mStudySum;
    QList<TauUModel> tauStudyList;

    QChart *chart;
    QValueAxis *axisX;
    QCategoryAxis *axisY;

public slots:

protected:
    void buildMenus();

public slots:
    void cut();
    void copy();
    void paste();
    void pasteInverted();
    void clear();

    void openRecentFile();

    bool eventFilter(QObject *object, QEvent *event);

    void LaunchStudySeriesFigure(QList<TauUModel> mTauList, TauUModel mOmnibus);
    void LaunchIndividualSeriesFigure(QList<TauUModel> mTauList, TauUModel mOmnibus);
    void LoadIndividualDataIntoChart(double y, double x1, double x2, double x3, int pairs, int lowestPairs);

    void saveSettings();

    void clearSheet();
    void showOpenFileDialog();
    void showSaveFileDialog();

    void showTauUWindow();
    void showGnomeLicenseWindow();
    void showQTLicenseWindow();

    void showCreditsWindow();
    void showFAQWindow();

    bool isToolWindowShown();

    void closeEvent(QCloseEvent* event);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();


private:
    QAction *newSheetAction;
    QAction *openSheetAction;
    QAction *saveSheetAction;
    QAction *exitSheetAction;

    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *clearAction;

    QAction *pasteInvertedAction;

    QAction *openAnalysisWindow;

    QAction *openTauUWindow;
    QAction *openLicenseGnome;

    QAction *openLicenseDMS;
    QAction *openLicenseQt;

    QAction *openAbout;
    QAction *openFAQ;

    QTableWidget *table;

    SheetSelectDialog *sheetSelectDialog;
    LicenseDialog *licenseDialog;


    TauDialog *tauWindow;

    AboutDialog *aboutDialog;
    CreditsDialog *creditsDialog;

    ResultsDialog *resultsDialog;

    QString settingsFile;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];

    QString strippedName(const QString &fullFileName);
    QString curFile;

    QAction *separatorAct;

    QStringList studyTags;
    QStringList idTags;
    QStringList phaseTags;

    TauUCalculations *calculator;

};


#endif // SHEETWIDGET_H
