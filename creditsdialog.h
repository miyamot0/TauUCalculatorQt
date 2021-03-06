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

#ifndef CREDITSDIALOG_H
#define CREDITSDIALOG_H

#include <QDialog>

namespace Ui {
class CreditsDialog;
}

class CreditsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreditsDialog(QWidget *parent = 0);
    ~CreditsDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreditsDialog *ui;
};

#endif // CREDITSDIALOG_H
