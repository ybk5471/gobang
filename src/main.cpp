#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../include/Ai.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath("qml/");
    engine.load("qml/main.qml");

    engine.rootContext()->setContextProperty("Ai", new Ai());

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
