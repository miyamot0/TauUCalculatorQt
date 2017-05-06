#include <QApplication>
#include <QDir>

#include "sheetwidget.h"

#ifdef _WIN32
    #include <QSettings>
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SheetWidget mNewSheet;
    mNewSheet.setWindowIcon(QPixmap(":/images/applications-other.png"));
    mNewSheet.show();

    return app.exec();
}
