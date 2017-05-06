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
