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

#include <QtXlsx>
#include <QDir>
#include <QFileDialog>
#include <QDesktopWidget>

#include "resultsdialog.h"
#include "ui_resultsdialog.h"
#include "sheetwidget.h"

ResultsDialog::ResultsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultsDialog)
{
    ui->setupUi(this);

    this->window()->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            window()->size(),
            qApp->desktop()->availableGeometry()
        )
    );
}

void ResultsDialog::ImportDataAndShow()
{
    SheetWidget *temp = qobject_cast <SheetWidget *>(parent());

    QStringList columnList;

    columnList << "";
    columnList << "Study Name";
    columnList << "Participant Name";
    columnList << "Baseline Correction";
    columnList << "Kendall's S";
    columnList << "Pairs";
    columnList << "Ties";
    columnList << "Kendall's Tau";
    columnList << "Lower CI";
    columnList << "Upper CI";
    columnList << "SD (Tau)";
    columnList << "Kendall's Tau-B";
    columnList << "Z-Score";
    columnList << "p Value";

    // Create columns
    for(int i=0; i<columnList.count();i++)
    {
        ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
        ui->tableWidget->setHorizontalHeaderItem(ui->tableWidget->columnCount() - 1, new QTableWidgetItem(columnList.at(i)));
    }

    for(int i=0; i<temp->allResults.count(); i++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        QStringList mList = temp->allResults.at(i);

        for(int j=0; j<mList.count(); j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(mList.at(j));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, j, item);
        }
    }

    ui->tableWidget->viewport()->update();

    show();
}

ResultsDialog::~ResultsDialog()
{
    delete ui;
}

void ResultsDialog::on_pushButton_2_clicked()
{
    QString selFilter="Spreadsheet (*.xlsx)";
    QString file_name = QFileDialog::getSaveFileName(this, "Save file", QDir::currentPath(), "Spreadsheet (*.xlsx)", &selFilter);

    if(!file_name.trimmed().isEmpty())
    {
        QXlsx::Document xlsx;

        int rows = ui->tableWidget->rowCount();
        int cols = ui->tableWidget->columnCount();

        QString temp;
        QTableWidgetItem *col;

        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<cols; j++)
            {
                if (i == 0)
                {
                    col = ui->tableWidget->horizontalHeaderItem(j);
                    temp = col->text();
                    xlsx.write(1, j + 1, temp);
                }

                temp = ui->tableWidget->item(i, j)->data(Qt::DisplayRole).toString();
                xlsx.write(i + 2, j + 1, temp);
            }
        }

        xlsx.saveAs(file_name);
    }
}

void ResultsDialog::convertExcelColumn(QString &mString, int column)
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
