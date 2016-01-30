#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "gui/pdoc.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<PDoc>("sell", 1, 0, "PDoc");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
