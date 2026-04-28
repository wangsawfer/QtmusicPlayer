#include "stdafx.h"
#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    qputenv("QTWEBENGINE_DISABLE_SANDBOX", QByteArray("1"));
    QApplication app(argc, argv);
    mainwindow window;
    window.show();
    return app.exec();
}
