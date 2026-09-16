#include "ecu/FakeEcuClient.h"
#include "nav/GpsdClient.h"
#include "nav/SimulatedNavEngine.h"
#include "bt/BluezController.h"

#ifdef EVO_HMI_HAS_OSMSCOUT
#include "nav/OsmScoutNavEngine.h"
#endif

#include <QCommandLineParser>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#ifdef EVO_HMI_HAS_QUICKCONTROLS
#include <QQuickStyle>
#endif

static void maybeSelectEglfs()
{
#ifdef Q_OS_LINUX
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")
        && qEnvironmentVariableIsEmpty("DISPLAY")
        && qEnvironmentVariableIsEmpty("WAYLAND_DISPLAY")) {
        qputenv("QT_QPA_PLATFORM", "eglfs");
    }
#endif
}

int main(int argc, char *argv[])
{
    maybeSelectEglfs();

    QGuiApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("evo-hmi"));
    app.setOrganizationName(QStringLiteral("evo"));
    app.setApplicationVersion(QStringLiteral("0.1.0"));

#ifdef EVO_HMI_HAS_QUICKCONTROLS
    QQuickStyle::setStyle(QStringLiteral("Basic"));
#endif

    // Solid and Regular share family "Font Awesome 7 Free"; distinguish by weight/styleName.
    QFontDatabase::addApplicationFont(QStringLiteral(":/qt/qml/EvoHmi/qml/fonts/fa-solid-900.otf"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/qt/qml/EvoHmi/qml/fonts/fa-regular-400.otf"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/qt/qml/EvoHmi/qml/fonts/fa-brands-400.otf"));

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Evo infotainment HMI"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption fakeBt(QStringLiteral("fake-bt"), QStringLiteral("Force simulated Bluetooth"));
    QCommandLineOption fakeGps(QStringLiteral("fake-gps"), QStringLiteral("Force simulated GPS (ignore gpsd)"));
    QCommandLineOption gpsdHost(QStringLiteral("gpsd-host"), QStringLiteral("gpsd host"), QStringLiteral("host"),
                                QStringLiteral("127.0.0.1"));
    QCommandLineOption gpsdPort(QStringLiteral("gpsd-port"), QStringLiteral("gpsd port"), QStringLiteral("port"),
                                QStringLiteral("2947"));
    QCommandLineOption mapDir(QStringLiteral("map-dir"), QStringLiteral("libosmscout map directory"), QStringLiteral("dir"));
    parser.addOption(fakeBt);
    parser.addOption(fakeGps);
    parser.addOption(gpsdHost);
    parser.addOption(gpsdPort);
    parser.addOption(mapDir);
    parser.process(app);

    auto *ecu = new FakeEcuClient(&app);

    auto *gpsd = new GpsdClient(&app);
    gpsd->setHost(parser.value(gpsdHost), parser.value(gpsdPort).toUShort());
    if (parser.isSet(fakeGps))
        gpsd->startSimulation();
    else
        gpsd->connectToDaemon();

    NavEngine *nav = nullptr;
#ifdef EVO_HMI_HAS_OSMSCOUT
    nav = new OsmScoutNavEngine(gpsd, parser.value(mapDir), &app);
#else
    nav = new SimulatedNavEngine(gpsd, &app);
#endif

    auto *bt = new BluezController(&app);
    bt->setForceSimulated(parser.isSet(fakeBt));
    bt->start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("ecu"), ecu);
    engine.rootContext()->setContextProperty(QStringLiteral("nav"), nav);
    engine.rootContext()->setContextProperty(QStringLiteral("bt"), bt);
    engine.rootContext()->setContextProperty(QStringLiteral("platformName"), QGuiApplication::platformName());

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    engine.loadFromModule("EvoHmi", "Main");
    return app.exec();
}
