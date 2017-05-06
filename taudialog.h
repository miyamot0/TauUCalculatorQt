#ifndef TAUDIALOG_H
#define TAUDIALOG_H

#include <QDialog>

namespace Ui {
class TauDialog;
}

class TauDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TauDialog(QWidget *parent = 0);
    ~TauDialog();

private:
    Ui::TauDialog *ui;
};

#endif // TAUDIALOG_H
