#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "topcounter.h"
#include "wordsmodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    TopCounter counter;
    WordsModel wordsModel;

    counter.setWordsModel(&wordsModel);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("counter", &counter);
    engine.rootContext()->setContextProperty("wordsModel", &wordsModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
