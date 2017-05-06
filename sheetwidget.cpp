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

   The SheetWidget class was inspired by the Qt 5.8 Spreadsheet example and Recent Files example, its license is below:

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

#include <QtWidgets>
#include <QTableWidgetItem>
#include <QtXlsx>
#include <QtCharts>

#include "sheetwidget.h"
#include "resultsdialog.h"
#include "taudialog.h"
#include "tauumodel.h"
#include "tauucalculations.h"

#include <QDebug>

QTXLSX_USE_NAMESPACE

/**
 * @brief SheetWidget::SheetWidget
 * @param parent
 */
SheetWidget::SheetWidget(QWidget *parent) : QMainWindow(parent)
{
    table = new QTableWidget(10000, 10000, this);
    table->setSizeAdjustPolicy(QTableWidget::AdjustToContents);

    QString value;

    for (int c = 0; c < 10000; ++c)
    {
        if (c == 0)
        {
            table->setHorizontalHeaderItem(0, new QTableWidgetItem("Study Tag"));
        }
        else if (c == 1)
        {
            table->setHorizontalHeaderItem(1, new QTableWidgetItem("Subject Tag"));
        }
        else if (c == 2)
        {
            table->setHorizontalHeaderItem(2, new QTableWidgetItem("Phase Tag"));
        }
        else
        {
            value = "";
            convertExcelColumn(value, c - 3);
            table->setHorizontalHeaderItem(c, new QTableWidgetItem(value));
        }
    }

    buildMenus();
    setCentralWidget(table);

    setWindowTitle("Tau-U Calculator v 1.0.0.0");

    this->layout()->setSizeConstraint(QLayout::SetNoConstraint);

    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    #ifdef _WIN32

    this->window()->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            window()->size(),
            qApp->desktop()->availableGeometry()
        )
    );

    #endif

    table->installEventFilter(this);
}

/**
 * @brief SheetWidget::LaunchStudySeriesFigure
 * @param mTauList
 * @param mOmnibus
 */
void SheetWidget::LaunchStudySeriesFigure(QList<TauUModel> mTauList, TauUModel mOmnibus)
{
    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("Study-level and Omnibus Tau-U Metrics");

    axisX = new QValueAxis;
    axisX->gridLineColor();
    axisX->setTickCount(5);
    axisX->setMin(-2.0);
    axisX->setMax(2.0);
    axisX->setLinePenColor(Qt::black);
    axisX->setLinePen(QPen(Qt::black));
    axisY = new QCategoryAxis;

    int mCounter = 1;

    axisY->append("Omnibus", mCounter);

    for (int i = mTauList.count() - 1; i >= 0; i--)
    {
        axisY->append(mTauList[i].StudyName, mCounter + 1);
        mCounter++;
    }

    axisY->setRange(0, mTauList.count() + 1);
    axisY->setMin(0);
    axisY->setMax(mTauList.count() + 1);
    axisY->setLinePenColor(Qt::white);
    axisY->setLinePen(QPen(Qt::white));

    axisX->setGridLineVisible(false);
    axisY->setGridLineVisible(false);

    double mHeight = ((double) mCounter) + 0.5;

    int lowestNumber = 99999999;

    foreach(TauUModel tau, mTauList)
    {
        if (tau.Pairs < lowestNumber)
        {
            lowestNumber = tau.Pairs;
        }
    }

    foreach(TauUModel tau, mTauList)
    {
        mHeight -= 1;

        LoadIndividualDataIntoChart(mHeight, tau.CI[0], tau.TAU, tau.CI[1], tau.Pairs, lowestNumber);
    }

    mHeight -= 1;

    LoadIndividualDataIntoChart(mHeight, mOmnibus.CI[0], mOmnibus.TAU, mOmnibus.CI[1], 99999, lowestNumber);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow *window = new QMainWindow(this);
    window->setWindowTitle("Study-level and Omnibus Tau-U");
    window->setCentralWidget(chartView);
    window->resize(800, 600);
    window->show();
}

/**
 * @brief SheetWidget::LaunchIndividualSeriesFigure
 * @param mTauList
 * @param mOmnibus
 */
void SheetWidget::LaunchIndividualSeriesFigure(QList<TauUModel> mTauList, TauUModel mOmnibus)
{
    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("Individual and Omnibus Tau-U Metrics");

    axisX = new QValueAxis;
    axisX->gridLineColor();
    axisX->setTickCount(5);
    axisX->setMin(-2.0);
    axisX->setMax(2.0);
    axisX->setLinePenColor(Qt::black);
    axisX->setLinePen(QPen(Qt::black));
    axisY = new QCategoryAxis;

    int mCounter = 1;

    axisY->append("Omnibus", mCounter);

    for (int i = mTauList.count() - 1; i >= 0; i--)
    {
        axisY->append(mTauList[i].StudyName + ": " + mTauList[i].ParticipantName, mCounter + 1);
        mCounter++;
    }

    axisY->setRange(0, mTauList.count() + 1);
    axisY->setMin(0);
    axisY->setMax(mTauList.count() + 1);
    axisY->setLinePenColor(Qt::white);
    axisY->setLinePen(QPen(Qt::white));

    axisX->setGridLineVisible(false);
    axisY->setGridLineVisible(false);

    double mHeight = ((double) mCounter) + 0.5;

    int lowestNumber = 99999999;

    foreach(TauUModel tau, mTauList)
    {
        if (tau.Pairs < lowestNumber)
        {
            lowestNumber = tau.Pairs;
        }
    }

    foreach(TauUModel tau, mTauList)
    {
        mHeight -= 1;
        LoadIndividualDataIntoChart(mHeight, tau.CI[0], tau.TAU, tau.CI[1], tau.Pairs, lowestNumber);
    }

    mHeight -= 1;

    LoadIndividualDataIntoChart(mHeight, mOmnibus.CI[0], mOmnibus.TAU, mOmnibus.CI[1], 99999, lowestNumber);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow *window = new QMainWindow(this);
    window->setWindowTitle("Individual and Omnibus Tau-U");
    window->setCentralWidget(chartView);
    window->resize(800, 600);
    window->show();
}

/**
 * @brief SheetWidget::LoadIndividualDataIntoChart
 * @param y
 * @param x1
 * @param x2
 * @param x3
 * @param pairs
 * @param lowestPairs
 */
void SheetWidget::LoadIndividualDataIntoChart(double y, double x1, double x2, double x3, int pairs, int lowestPairs)
{
    if (pairs == 99999)
    {
        QLineSeries *series = new QLineSeries();
        series->setPen(QPen(Qt::black));
        chart->addSeries(series);
        *series << QPointF(x1, y) <<
                   QPointF(x2, y + 0.2) <<
                   QPointF(x3, y) <<
                   QPointF(x2, y - 0.2) <<
                   QPointF(x1, y);


        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);
    }
    else
    {
        int weight = qExp(qLn(10) + qLn((double) (pairs / lowestPairs)));

        QScatterSeries *series0 = new QScatterSeries();
        series0->setName("point");
        series0->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
        series0->setMarkerSize(weight);
        series0->setPen(QPen(Qt::black));
        series0->setBrush(QBrush(Qt::black));
        chart->addSeries(series0);

        series0->append(x2, y);

        chart->setAxisX(axisX, series0);
        chart->setAxisY(axisY, series0);

        QLineSeries *series = new QLineSeries();
        series->setPen(QPen(Qt::black));
        chart->addSeries(series);
        *series << QPointF(x1, y) << QPointF(x1, y + 0.05) << QPointF(x1, y - 0.05) << QPointF(x1, y) <<
                   QPointF(x2, y) <<
                   QPointF(x3, y) << QPointF(x3, y + 0.05) << QPointF(x3, y - 0.05) << QPointF(x3, y);


        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);
    }
}

/**
 * @brief SheetWidget::buildMenus
 */
void SheetWidget::buildMenus()
{
    /** File actions
     * @brief
     */

    newSheetAction = new QAction("N&ew Sheet", this);
    newSheetAction->setIcon(QIcon(":/images/document-new.png"));
    connect(newSheetAction, &QAction::triggered, this, &SheetWidget::clearSheet);

    openSheetAction = new QAction("I&mport a Sheet", this);
    openSheetAction->setShortcut(QKeySequence("Ctrl+O"));
    openSheetAction->setIcon(QIcon(":/images/document-open.png"));
    connect(openSheetAction, &QAction::triggered, this, &SheetWidget::showOpenFileDialog);

    saveSheetAction = new QAction("S&ave Sheet", this);
    saveSheetAction->setShortcut(QKeySequence("Ctrl+S"));
    saveSheetAction->setIcon(QIcon(":/images/document-save.png"));
    connect(saveSheetAction, &QAction::triggered, this, &SheetWidget::showSaveFileDialog);

    exitSheetAction = new QAction("E&xit", this);
    exitSheetAction->setShortcut(QKeySequence("Ctrl+Q"));
    exitSheetAction->setIcon(QIcon(":/images/application-exit.png"));
    connect(exitSheetAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    /** Window actions
     * @brief
     */

    openTauUWindow = new QAction("C&alculate Tau-U", this);
    openTauUWindow->setIcon(QIcon(":/images/applications-other.png"));
    connect(openTauUWindow, &QAction::triggered, this, &SheetWidget::showTauUWindow);

    /** Edit actions
     * @brief
     */

    cutAction = new QAction("Cut", this);
    cutAction->setShortcut(QKeySequence("Ctrl+X"));
    cutAction->setIcon(QIcon(":/images/edit-cut.png"));
    connect(cutAction, &QAction::triggered, this, &SheetWidget::cut);

    copyAction = new QAction("Copy", this);
    copyAction->setShortcut(QKeySequence("Ctrl+C"));
    copyAction->setIcon(QIcon(":/images/edit-copy.png"));
    connect(copyAction, &QAction::triggered, this, &SheetWidget::copy);

    pasteAction = new QAction("Paste", this);
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    pasteAction->setIcon(QIcon(":/images/edit-paste.png"));
    connect(pasteAction, &QAction::triggered, this, &SheetWidget::paste);

    pasteInvertedAction = new QAction("Paste Transposed", this);
    pasteInvertedAction->setShortcut(QKeySequence("Ctrl+B"));
    pasteInvertedAction->setIcon(QIcon(":/images/edit-paste.png"));
    connect(pasteInvertedAction, &QAction::triggered, this, &SheetWidget::pasteInverted);

    clearAction = new QAction("Clear", this);
    clearAction->setShortcut(Qt::Key_Delete);
    clearAction->setIcon(QIcon(":/images/edit-clear.png"));
    connect(clearAction, &QAction::triggered, this, &SheetWidget::clear);

    /** Window actions
     * @brief
     */

    openLicenseQt = new QAction("Qt License (LGPL-V3)", this);
    openLicenseQt->setIcon(QIcon(":/images/text-x-generic.png"));
    connect(openLicenseQt, &QAction::triggered, this, &SheetWidget::showQTLicenseWindow);

    openLicenseGnome = new QAction("Gnome Icons License (GPL-V3)", this);
    openLicenseGnome->setIcon(QIcon(":/images/text-x-generic.png"));
    connect(openLicenseGnome, &QAction::triggered, this, &SheetWidget::showGnomeLicenseWindow);

    openAbout = new QAction("Credits", this);
    openAbout->setIcon(QIcon(":/images/text-x-generic.png"));
    connect(openAbout, &QAction::triggered, this, &SheetWidget::showCreditsWindow);

    openFAQ = new QAction("FAQ", this);
    openFAQ->setIcon(QIcon(":/images/text-x-generic.png"));
    connect(openFAQ, &QAction::triggered, this, &SheetWidget::showFAQWindow);

    /** Window helper actions
     * @brief
     */

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    /** Menu area below
     * @brief
     */

    QMenu *sheetOptionsMenu = menuBar()->addMenu(tr("&File"));
    sheetOptionsMenu->addAction(newSheetAction);
    sheetOptionsMenu->addAction(openSheetAction);
    sheetOptionsMenu->addAction(saveSheetAction);

    separatorAct = sheetOptionsMenu->addSeparator();

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        sheetOptionsMenu->addAction(recentFileActs[i]);
    }

    sheetOptionsMenu->addSeparator();

    sheetOptionsMenu->addAction(exitSheetAction);

    updateRecentFileActions();

    QMenu *sheetEditMenu = menuBar()->addMenu(tr("&Edit"));
    sheetEditMenu->addAction(cutAction);
    sheetEditMenu->addAction(copyAction);
    sheetEditMenu->addAction(pasteAction);
    sheetEditMenu->addAction(pasteInvertedAction);
    sheetEditMenu->addSeparator();
    sheetEditMenu->addAction(clearAction);

    QMenu *sheetCalculationsMenu = menuBar()->addMenu(tr("C&alculations"));
    sheetCalculationsMenu->addAction(openTauUWindow);

    QMenu *sheetLicensesMenu = menuBar()->addMenu(tr("&Licenses"));
    sheetLicensesMenu->addAction(openLicenseQt);
    sheetLicensesMenu->addAction(openLicenseGnome);
    sheetLicensesMenu->addAction(openAbout);

    QMenu *sheetAboutMenu = menuBar()->addMenu(tr("&Help"));
    sheetAboutMenu->addAction(openFAQ);

    /** Context Menu
     * @brief
     */

    QAction *separatorTwo = new QAction(this);
    separatorTwo->setSeparator(true);
    addAction(separatorTwo);

    addAction(cutAction);
    addAction(copyAction);
    addAction(pasteAction);
    addAction(pasteInvertedAction);

    QAction *separatorThree = new QAction(this);
    separatorThree->setSeparator(true);
    addAction(separatorThree);

    addAction(clearAction);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

/**
 * @brief SheetWidget::clearSheet
 */
void SheetWidget::clearSheet()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    table->clearContents();

    curFile = "";
    setWindowFilePath(curFile);

    QApplication::setOverrideCursor(Qt::WaitCursor);
}

/**
 * @brief SheetWidget::closeEvent
 * @param event
 */
void SheetWidget::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton confirm = QMessageBox::question( this, "Tau-U Calculator",
                                                                tr("Are you sure you want to quit?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (confirm != QMessageBox::Yes) {
        event->ignore();
    } else {
        saveSettings();
        event->accept();
    }
}

/**
 * @brief SheetWidget::openRecentFile
 */
void SheetWidget::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString mFile = action->data().toString();

        if (QFile::exists(mFile))
        {
            Document xlsx2(mFile);
            QStringList sheets = xlsx2.sheetNames();

            sheetSelectDialog = new SheetSelectDialog(this);
            sheetSelectDialog->UpdateUI(sheets);
            sheetSelectDialog->setModal(true);

            if(sheetSelectDialog->exec())
            {
                table->clearContents();

                xlsx2.selectSheet(sheetSelectDialog->GetSelected());

                for (int w = 0; w < xlsx2.dimension().lastColumn() + 1; w++)
                {
                    for (int h = 0; h < xlsx2.dimension().lastRow() + 1; h++)
                    {
                        if (QXlsx::Cell *cell = xlsx2.cellAt(h, w))
                        {
                            if (cell->cellType() == Cell::NumberType || cell->cellType() == Cell::StringType || cell->cellType() == Cell::SharedStringType)
                            {
                                table->setItem(h-1, w-1, new QTableWidgetItem(cell->value().toString()));
                            }
                        }
                    }
                }

                setCurrentFile(mFile);
                statusBar()->showMessage(tr("File loaded"), 2000);
            }
        }
    }
}

/**
 * @brief SheetWidget::setCurrentFile
 * @param fileName
 */
void SheetWidget::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowFilePath(curFile);

    QSettings settings(QSettings::UserScope, QLatin1String("Tau-U Calculator"));
    settings.beginGroup(QLatin1String("SheetWindow"));

    QStringList files = settings.value(QLatin1String("recentFileList")).toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);

    while (files.size() > MaxRecentFiles)
    {
        files.removeLast();
    }

    settings.setValue("recentFileList", files);
    settings.endGroup();
    settings.sync();

    updateRecentFileActions();
}

/**
 * @brief SheetWidget::saveSettings
 */
void SheetWidget::saveSettings()
{
    QSettings settings(QSettings::UserScope, QLatin1String("Tau-U Calculator"));
    settings.beginGroup(QLatin1String("SheetWindow"));

    QStringList files = settings.value("recentFileList").toStringList();

    settings.setValue(QLatin1String("recentFileList"), files);
    settings.endGroup();

    settings.sync();
}

/**
 * @brief SheetWidget::eventFilter
 * @param object
 * @param event
 * @return
 */
bool SheetWidget::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        auto keyCode = static_cast<QKeyEvent *>(event);
        if (keyCode->key() == (int) Qt::Key_Return)
        {
            if (table->currentRow() + 1 >= table->rowCount())
            {
                return QObject::eventFilter(object, event);
            }

            table->setCurrentCell(table->currentRow() + 1, table->currentColumn());
        }
    }

    return QObject::eventFilter(object, event);
}

/**
 * @brief SheetWidget::showOpenFileDialog
 */
void SheetWidget::showOpenFileDialog()
{
    QString file_name;
    QString fileFilter = "Spreadsheet (*.xlsx)";

    #ifdef _WIN32

    file_name = QFileDialog::getOpenFileName(this, "Open spreadsheet file", QDir::homePath(),
                                             fileFilter);

    #elif TARGET_OS_MAC

    file_name = QFileDialog::getOpenFileName(nullptr, "Open spreadsheet file", QDir::homePath(),
                                             fileFilter, nullptr, QFileDialog::Option::DontUseNativeDialog);

    #endif

    if(!file_name.trimmed().isEmpty())
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        Document xlsx2(file_name);
        QStringList sheets = xlsx2.sheetNames();

        sheetSelectDialog = new SheetSelectDialog(this);
        sheetSelectDialog->UpdateUI(sheets);
        sheetSelectDialog->setModal(true);

        if(sheetSelectDialog->exec())
        {
            table->clearContents();

            xlsx2.selectSheet(sheetSelectDialog->GetSelected());

            for (int w = 0; w < xlsx2.dimension().lastColumn() + 1; w++)
            {
                for (int h = 0; h < xlsx2.dimension().lastRow() + 1; h++)
                {
                    if (QXlsx::Cell *cell = xlsx2.cellAt(h, w))
                    {
                        if (cell->cellType() == Cell::NumberType || cell->cellType() == Cell::StringType || cell->cellType() == Cell::SharedStringType)
                        {
                            table->setItem(h-1, w-1, new QTableWidgetItem(cell->value().toString()));
                        }
                    }
                }
            }

            setCurrentFile(file_name);
            statusBar()->showMessage(tr("File loaded"), 2000);
        }

        QApplication::restoreOverrideCursor();
    }
}

/**
 * @brief SheetWidget::updateRecentFileActions
 */
void SheetWidget::updateRecentFileActions()
{
    QSettings settings(QSettings::UserScope, QLatin1String("Tau-U Calculator"));
    settings.beginGroup(QLatin1String("SheetWindow"));
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }

    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
    {
        recentFileActs[j]->setVisible(false);
    }

    separatorAct->setVisible(numRecentFiles > 0);
}

/**
 * @brief SheetWidget::showSaveFileDialog
 */
void SheetWidget::showSaveFileDialog()
{

    QString file_name;
    QString fileFilter = "Spreadsheet (*.xlsx)";

#ifdef _WIN32

        file_name = QFileDialog::getSaveFileName(this, "Save spreadsheet file", QDir::homePath(),
                                         fileFilter);

#elif TARGET_OS_MAC

        file_name = QFileDialog::getSaveFileName(this, "Save spreadsheet file", QDir::homePath(),
                                         fileFilter, &fileFilter, QFileDialog::Option::DontUseNativeDialog);

        if (!file_name.contains(".xlsx"))
        {
            file_name.append(".xlxs");
        }

#endif

    if(!file_name.trimmed().isEmpty())
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        QXlsx::Document xlsx;

        int rows = table->rowCount();
        int cols = table->columnCount();

        QString temp;

        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<cols; j++)
            {
                QTableWidgetItem *item = table->item(i, j);

                if (item != NULL && !item->text().isEmpty())
                {
                    temp = table->item(i, j)->data(Qt::DisplayRole).toString();
                    xlsx.write(i + 1, j + 1, temp);
                }
            }
        }

        xlsx.saveAs(file_name);
        setCurrentFile(file_name);

        QApplication::restoreOverrideCursor();

        statusBar()->showMessage(tr("File saved"), 2000);
    }
}

/**
 * @brief SheetWidget::showTauUWindow
 */
void SheetWidget::showTauUWindow()
{
    tauWindow = new TauDialog(this);
    tauWindow->setModal(false);
    tauWindow->show();
}

/**
 * @brief SheetWidget::showQTLicenseWindow
 */
void SheetWidget::showQTLicenseWindow()
{
    QString mFilePath = "";

    #ifdef _WIN32
            mFilePath = "License_Qt.txt";
    #elif TARGET_OS_MAC
            QDir runDirectory = QDir(QCoreApplication::applicationDirPath());
            runDirectory.cdUp();
            runDirectory.cd("Resources");
            mFilePath = "\"" + runDirectory.path() + "/";

            mFilePath = mFilePath + scriptName + "License_Qt.txt\"";

    #endif

    licenseDialog = new LicenseDialog(mFilePath, this);
    licenseDialog->setWindowTitle("Qt License (LGPL-V3)");
    licenseDialog->setModal(true);
    licenseDialog->show();
}

/**
 * @brief SheetWidget::showGnomeLicenseWindow
 */
void SheetWidget::showGnomeLicenseWindow()
{
    QString mFilePath = "";

    #ifdef _WIN32
            mFilePath = "License_gnome_icons.txt";
    #elif TARGET_OS_MAC
            QDir runDirectory = QDir(QCoreApplication::applicationDirPath());
            runDirectory.cdUp();
            runDirectory.cd("Resources");
            mFilePath = "\"" + runDirectory.path() + "/";

            mFilePath = mFilePath + scriptName + "License_gnome_icons.txt\"";

    #endif

    licenseDialog = new LicenseDialog(mFilePath, this);
    licenseDialog->setWindowTitle("Gnome Icon Set License (GPL-V3)");
    licenseDialog->setModal(true);
    licenseDialog->show();
}

/**
 * @brief SheetWidget::showCreditsWindow
 */
void SheetWidget::showCreditsWindow()
{
    creditsDialog = new CreditsDialog();
    creditsDialog->setModal(true);
    creditsDialog->show();
}

/**
 * @brief SheetWidget::showFAQWindow
 */
void SheetWidget::showFAQWindow()
{
    aboutDialog = new AboutDialog();
    aboutDialog->setModal(true);
    aboutDialog->show();
}

/**
 * @brief SheetWidget::cut
 */
void SheetWidget::cut()
{
    SheetWidget::copy();
    SheetWidget::clear();
}

/**
 * @brief SheetWidget::copy
 */
void SheetWidget::copy()
{
    QList<QTableWidgetSelectionRange> range = table->selectedRanges();
    QTableWidgetSelectionRange mRange = range.first();

    QString str;

    for (int i = 0; i < mRange.rowCount(); ++i) {
        if (i > 0)
        {
            str += "\n";
        }

        for (int j = 0; j < mRange.columnCount(); ++j) {
            if (j > 0)
            {
                str += "\t";
            }

            if (table->item(mRange.topRow() + i, mRange.leftColumn() + j) == NULL)
            {
                str += "";
            }
            else
            {
                str += table->item(mRange.topRow() + i, mRange.leftColumn() + j)->data(Qt::DisplayRole).toString();
            }
        }
    }

    QApplication::clipboard()->setText(str);
}

/**
 * @brief SheetWidget::paste
 */
void SheetWidget::paste()
{
    QTableWidgetSelectionRange range = table->selectedRanges().first();
    QString pasteString = QApplication::clipboard()->text();
    QStringList pasteRows = pasteString.split('\n');

    int nRows = pasteRows.count();
    int nCols = pasteRows.first().count('\t') + 1;

    for (int i = 0; i < nRows; ++i) {
        QStringList columns = pasteRows[i].split('\t');

        for (int j = 0; j < nCols; ++j) {
            int row = range.topRow() + i;
            int column = range.leftColumn() + j;

            if (row < 10000 && column < 10000)
            {
                if (table->item(row, column) != NULL)
                {
                    if (j < columns.length())
                    {
                        table->item(row, column)->setText(columns[j]);
                    }
                }
                else
                {
                    if (j < columns.length())
                    {
                        table->setItem(row, column, new QTableWidgetItem(columns[j]));
                    }
                }
            }
        }
    }

    table->viewport()->update();
}

/**
 * @brief SheetWidget::pasteInverted
 */
void SheetWidget::pasteInverted()
{
    QTableWidgetSelectionRange range = table->selectedRanges().first();
    QString pasteString = QApplication::clipboard()->text();
    QStringList pasteRows = pasteString.split('\n');

    int nRows = pasteRows.count();
    int nCols = pasteRows.first().count('\t') + 1;

    for (int i = 0; i < nRows; ++i) {
        QStringList columns = pasteRows[i].split('\t');

        for (int j = 0; j < nCols; ++j) {
            int row = range.topRow() + j;
            int column = range.leftColumn() + i;

            if (row < 10000 && column < 10000)
            {
                if (table->item(row, column) != NULL)
                {
                    if (j < columns.length())
                    {
                        table->item(row, column)->setText(columns[j]);
                    }
                }
                else
                {
                    if (j < columns.length())
                    {
                        table->setItem(row, column, new QTableWidgetItem(columns[j]));
                    }
                }
            }
        }
    }

    table->viewport()->update();
}

/**
 * @brief SheetWidget::clear
 */
void SheetWidget::clear()
{
    foreach (QTableWidgetItem *i, table->selectedItems())
    {
        i->setText("");
    }
}

/**
 * @brief SheetWidget::isToolWindowShown
 * @return
 */
bool SheetWidget::isToolWindowShown()
{
    /*
    if (discountingAreaDialog->isVisible())
    {
        return true;
    }
    else if (discountingED50Dialog->isVisible())
    {
        return true;
    }
    */

    return false;
}

/**
 * @brief SheetWidget::ParseData
 * @param correct
 * @param threshold
 * @param confidence
 * @param figures
 */
void SheetWidget::ParseData(bool correct, double threshold, double confidence, QString figures)
{
    int rowTotals = 0, subjTotals = 0, phaseTotals = 0;
    allResults.clear();
    mStudySum.clear();
    tauStudyList.clear();
    studyTags.clear();
    idTags.clear();
    phaseTags.clear();

    QString temp;
    QTableWidgetItem *item;

    // Parse all available data
    for (int i=0; i<1000; i++)
    {
        item = table->item(i, 0);

        if (item != NULL && !item->text().isEmpty())
        {
            temp = table->item(i, 0)->data(Qt::DisplayRole).toString();
            studyTags.append(temp);
            rowTotals++;
        }
        else
        {
            break;
        }

        item = table->item(i, 1);

        if (item != NULL && !item->text().isEmpty())
        {
            temp = table->item(i, 1)->data(Qt::DisplayRole).toString();
            idTags.append(temp);
            subjTotals++;
        }
        else
        {
            break;
        }

        item = table->item(i, 2);

        if (item != NULL && !item->text().isEmpty())
        {
            temp = table->item(i, 2)->data(Qt::DisplayRole).toString();
            phaseTags.append(temp);
            phaseTotals++;
        }
        else
        {
            break;
        }
    }

    if (rowTotals != subjTotals || subjTotals != phaseTotals)
    {
        QMessageBox::about(this, "Error", "The available data does not appear complete");

        return;
    }

    calculator = new TauUCalculations();
    QList<TauUModel> tauList;

    // Compute all comparisons
    for (int i=1; i<rowTotals; i++)
    {
        QTableWidgetItem *item;

        QString preStudy = table->item(i-1, 0)->data(Qt::DisplayRole).toString();
        QString preSubject = table->item(i-1, 1)->data(Qt::DisplayRole).toString();
        QString prePhase = table->item(i-1, 2)->data(Qt::DisplayRole).toString();

        QString postStudy = table->item(i, 0)->data(Qt::DisplayRole).toString();
        QString postSubject = table->item(i, 1)->data(Qt::DisplayRole).toString();
        QString postPhase = table->item(i, 2)->data(Qt::DisplayRole).toString();

        TauUModel tauModel;

        if (prePhase == "A" && postPhase == "B" &&
            preSubject == postSubject && preStudy == postStudy)
        {
            bool validateNumber;

            QList<double> baselines;
            QList<double> interventions;

            for (int j = 3; j < 1000; j++)
            {
                item = table->item(i-i, j);

                if (item != NULL && !item->text().isEmpty())
                {
                    temp = table->item(i-i, j)->data(Qt::DisplayRole).toString();

                    if(temp.trimmed() == "0")
                    {
                        baselines.append(0.0);
                    }
                    else if(temp.toDouble(&validateNumber))
                    {
                        baselines.append(temp.toDouble());
                    }
                }
                else
                {
                    break;
                }
            }

            for (int j = 3; j < 1000; j++)
            {
                item = table->item(i, j);

                if (item != NULL && !item->text().isEmpty())
                {
                    temp = table->item(i, j)->data(Qt::DisplayRole).toString();

                    if(temp.trimmed() == "0")
                    {
                        interventions.append(0.0);
                    }
                    else if(temp.toDouble(&validateNumber))
                    {
                        interventions.append(temp.toDouble());
                    }
                }
                else
                {
                    break;
                }
            }

            calculator->SetData(baselines, interventions, preStudy, preSubject, "Participant");

            tauModel = calculator->BaselineTrend();

            if (correct)
            {
                if (threshold != -1)
                {
                    if (qFabs(tauModel.TAU) >= qFabs(threshold))
                    {
                        tauModel = calculator->CompareTrend(true, confidence);
                    }
                    else
                    {
                        tauModel = calculator->CompareTrend(false, confidence);
                    }
                }
                else
                {
                    tauModel = calculator->CompareTrend(true, confidence);
                }
            }
            else
            {
                tauModel = calculator->CompareTrend(false, confidence);
            }

            tauList.append(tauModel);
        }
    }

    // Merge similar
    QList<TauUModel> tempList;
    for (int d = tauList.count() - 2; d >= 0; d--)
    {
        if (tauList[d].StudyName == tauList[d+1].StudyName &&
            tauList[d].ParticipantName == tauList[d+1].ParticipantName)
        {
            tempList.clear();
            tempList << tauList[d] << tauList[d+1];
            tauList[d] = CombineTaus(tempList, confidence, tauList[d].StudyName);
            tauList[d].ParticipantName = tauList[d+1].ParticipantName;

            if (tauList[d].IsCorrected || tauList[d+1].IsCorrected)
            {
                tauList[d].IsCorrected = true;
            }
            else
            {
                tauList[d].IsCorrected = false;
            }

            tauList.removeAt(d+1);
        }
    }

    // Merge shared tau's (e.g., same participants)
    resultsDialog = new ResultsDialog(this);
    resultsDialog->setModal(false);

    // After all studies pulled, remove duplicates
    studyTags.removeDuplicates();

    bool hasACorrectedValue;
    bool hasACorrectedValueSomewhere = false;
    foreach (QString studyName, studyTags)
    {
        hasACorrectedValue = false;
        mStudySum.clear();

        foreach (TauUModel tauModel, tauList) {
            if (tauModel.StudyName == studyName)
            {
                if (tauModel.IsCorrected)
                {
                    hasACorrectedValue = true;
                    hasACorrectedValueSomewhere = true;
                }
                mStudySum.append(tauModel);
            }
        }

        TauUModel mModel = CombineTaus(mStudySum, confidence, studyName);
        mModel.StudyName = studyName;
        mModel.IsCorrected = hasACorrectedValue;

        tauStudyList.append(mModel);
    }

    QStringList mLineEntry;
    mLineEntry << "Omnibus-level output:";
    allResults.append(mLineEntry);

    QString mTempBoolString;

    if (tauList.count() > 2)
    {
        TauUModel mOmniModel = CombineTaus(tauList, confidence, "---");
        mOmniModel.IsCorrected = hasACorrectedValueSomewhere;

        if (figures == "Individual")
        {
            LaunchIndividualSeriesFigure(tauList, mOmniModel);
        }
        else if (figures == "Study")
        {
            LaunchStudySeriesFigure(tauStudyList, mOmniModel);
        }

        mTempBoolString = (hasACorrectedValueSomewhere) ? "True" : "False";

        mLineEntry.clear();
        mLineEntry << "Omnibus" << "---" << "---" << mTempBoolString << QString::number(mOmniModel.S) <<
                      QString::number(mOmniModel.Pairs) << QString::number(mOmniModel.Ties) <<
                      QString::number(mOmniModel.TAU) << "---" << QString::number(mOmniModel.TAUB) <<
                      QString::number(mOmniModel.SDtau) << QString::number(mOmniModel.Z) <<
                      QString::number(mOmniModel.PValue);

        allResults.append(mLineEntry);

        mLineEntry.clear();
        mLineEntry << "";

        allResults.append(mLineEntry);
    }

    mLineEntry.clear();
    mLineEntry << "Study-level output:";
    allResults.append(mLineEntry);

    foreach (TauUModel tauModel, tauStudyList) {
        mTempBoolString = (tauModel.IsCorrected) ? "True" : "False";

        mLineEntry.clear();
        mLineEntry << "Study" << tauModel.StudyName << "---" << mTempBoolString << QString::number(tauModel.S) <<
                      QString::number(tauModel.Pairs) << QString::number(tauModel.Ties) <<
                      QString::number(tauModel.TAU) << "---" << QString::number(tauModel.TAUB) <<
                      QString::number(tauModel.SDtau) << QString::number(tauModel.Z) <<
                      QString::number(tauModel.PValue);

        allResults.append(mLineEntry);
    }

    mLineEntry.clear();
    mLineEntry << "";
    allResults.append(mLineEntry);

    mLineEntry.clear();
    mLineEntry << "Individual-level output:";
    allResults.append(mLineEntry);

    foreach (TauUModel tauModel, tauList) {
        mTempBoolString = (tauModel.IsCorrected) ? "True" : "False";

        mLineEntry.clear();
        mLineEntry << "Individual" << tauModel.StudyName << tauModel.ParticipantName << mTempBoolString << QString::number(tauModel.S) <<
                      QString::number(tauModel.Pairs) << QString::number(tauModel.Ties) <<
                      QString::number(tauModel.TAU) << "---" << QString::number(tauModel.TAUB) <<
                      QString::number(tauModel.SDtau) << QString::number(tauModel.Z) <<
                      QString::number(tauModel.PValue);

        allResults.append(mLineEntry);
    }

    mLineEntry.clear();
    mLineEntry << "";
    allResults.append(mLineEntry);

    resultsDialog->ImportDataAndShow();

}

/**
 * @brief SheetWidget::CombineTaus
 * @param selectedTaus
 * @param confidence
 * @param studyName
 * @return
 */
TauUModel SheetWidget::CombineTaus(QList<TauUModel> selectedTaus, double confidence, QString studyName)
{
    if (selectedTaus.count() == 1)
    {
        return selectedTaus.first();
    }

    // Based on Hedges' Optimal Weighting formula

    double globalTau = 0.0, globalSETau = 0.0;
    double inverseSd = 0.0, inverseSdSum = 0.0;

    bool isCorrected = false;

    foreach(TauUModel model, selectedTaus)
    {
        inverseSd = 1.0 / model.SDtau;
        inverseSdSum += inverseSd;
        globalTau += model.TAU * inverseSd;
        globalSETau += model.SDtau * model.SDtau;

        if (isCorrected)
        {
            isCorrected = true;
        }
    }

    globalTau = globalTau / inverseSdSum;
    globalSETau = qSqrt(globalSETau) / selectedTaus.count();

    double z = globalTau / globalSETau;

    double pval = calculator->GetPValueFromUDistribution(z);

    double totalInverseVariance = 0.0, totalTauES = 0.0;
    double pairs = 0, ties = 0, S = 0;

    foreach(TauUModel model, selectedTaus)
    {
        totalInverseVariance += (1.0 / (model.SDtau * model.SDtau));
        totalTauES += (model.TAU * (1.0 / (model.SDtau * model.SDtau)));

        pairs += model.Pairs;
        ties += model.Ties;
        S += model.S;
    }

    TauUModel omniTau;
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

    return omniTau;
}

/**
 * @brief SheetWidget::strippedName
 * @param fullFileName
 * @return
 */
QString SheetWidget::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

/**
 * @brief SheetWidget::convertExcelColumn
 * @param mString
 * @param column
 */
void SheetWidget::convertExcelColumn(QString &mString, int column)
{
    int dividend = column + 1;
    QString columnName = "";

    int modulo;

    while (dividend > 0)
    {
        modulo = (dividend - 1) % 26;
        columnName = new QString(65 + modulo) + columnName;
        dividend = (int)((dividend - modulo) / 26);
    }

    mString = columnName;
}

