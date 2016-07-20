/** main.cpp
  * guzhoudiaoke@126.com
  * 2011-08-02
  */

#include <QApplication>
#include <QTextCodec>
#include "mineWidget.h"

int main(int argc, char * argv[])
{
    QApplication app(argc,argv);

    QTranslator translator;
    translator.load("saolei_zh");
    app.installTranslator(&translator);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    MineWidget w;
    w.show();

    return app.exec();
}
