#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "core.h"
#include "dynamicimage.h"
#include "imagecontainer.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<ImageContainer>("io.qt.forum", 1, 0, "ImageContainer");
    qmlRegisterType<Core>("io.qt.forum", 1, 0, "Core");
    qmlRegisterType<DynamicImage>("io.qt.forum", 1, 0, "DynamicImage");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    Core core;
    engine.rootContext()->setContextProperty("core", &core);
    engine.loadFromModule("fractal-stranger", "Main");

    return app.exec();
}
