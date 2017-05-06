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
