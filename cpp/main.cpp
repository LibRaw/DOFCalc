/* DOFCalc v0.1
    (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
    The program is licensed under the terms of GNU General Public License v3 */

#include "dofcalcobject.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("DOFCalc");
    QCoreApplication::setOrganizationName("LibRaw LLC");
    QCoreApplication::setOrganizationDomain("libraw.org");
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    DOFCalculator calculator(&engine);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())  return -1;
    calculator.afterQMLStart(&app);
    QTimer::singleShot(2000, &calculator, SLOT(restoreVelocities())); // Do not know how to do it more elegant way, so timed
    return  app.exec();
}
