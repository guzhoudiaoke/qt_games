/** main.cpp
  * 孤舟钓客
  * 2011-08-01
  */

#include <QApplication>
#include <QTextCodec>
#include "mainwidget.h"

int main(int argc, char * argv[])
{
    QApplication app(argc,argv);

    QTranslator translator;
    translator.load("pintu_zh");
    app.installTranslator(&translator);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    MainWidget w;
    w.show();

    return app.exec();
}

