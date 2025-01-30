#include "meltaway.h"
#include <QSurfaceFormat>
#include <QCommandLineParser>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QSurfaceFormat fmt;
//    fmt.setVersion(3, 3);
//    fmt.setProfile(QSurfaceFormat::CoreProfile);
//    QSurfaceFormat::setDefaultFormat(fmt);

    MeltAway w;
    w.show();
    return a.exec();
}
