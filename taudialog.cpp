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

#include "sheetwidget.h"
#include "taudialog.h"
#include "ui_taudialog.h"

TauDialog::TauDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TauDialog)
{
    ui->setupUi(this);
}

TauDialog::~TauDialog()
{
    delete ui;
}

void TauDialog::on_correctCondition_toggled(bool checked)
{
    ui->correctLineValue->setEnabled(checked);
}

void TauDialog::on_pushButton_clicked()
{
    SheetWidget *temp = qobject_cast <SheetWidget *>(parent());

    bool correct = false;
    double threshold = -1;
    double confidence = -1;
    QString figures = "";

    if (ui->correctAll->isChecked())
    {
        correct = true;
    }
    else if (ui->correctCondition->isChecked())
    {
        bool check;

        threshold = ui->correctCondition->text().toDouble(&check);

        if (!check)
        {
            QMessageBox::about(this, "Error", "Please input a valid Tau threshold");
            return;
        }
        else
        {
            correct = true;
        }
    }

    if (ui->confidence85->isChecked())
    {
        confidence = 1.44;
    }
    else if (ui->confidence90->isChecked())
    {
        confidence = 1.645;
    }
    else if (ui->confidence95->isChecked())
    {
        confidence = 1.96;
    }

    if (ui->figuresIndividual->isChecked())
    {
        figures = "Individual";
    }
    else if (ui->figuresStudy->isChecked())
    {
        figures = "Study";
    }

    temp->ParseData(correct, threshold, confidence, figures);
}
