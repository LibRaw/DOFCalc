/* DOFCalc v0.1
    (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
    The program is licensed under the terms of GNU General Public License v3 */

#include "dofcalcobject.h"
#include <QGuiApplication>
#include <QQmlContext>
#include <QWindow>
#include <QScreen>

DOFCalculator::DOFCalculator(QQmlApplicationEngine *e, QObject* parent /* = 0 */) : QObject(parent), engine(e) {
    QSize screensz = QGuiApplication::screens().at(0)->availableSize();
    int fs = QSysInfo::productType() == QString("windows") ? 18 : qBound(8, qMin(screensz.width(), screensz.height()) / 32, 30);
    settings.loadSettings(fs);
    params.loadSettings();
    setFontAndHeading(settings.values[s_fontSize]);

    datamodels[mv_focal] = new focalModel();
    datamodels[mv_aperture] = new apertureModel();
    datamodels[mv_distance] = new distanceModel();

    foreach(const QString& mname, datamodels.keys())
    {
        datamodels[mname]->populate(settings.values);
        engine->rootContext()->setContextProperty(QString(t_Name2Model).arg(mname), datamodels[mname]);
    }
}

void DOFCalculator::setViewIndexAndShow(const QString &mname, const QString& object, QObject* flist) {
    int index = findNearestIndex(datamodels[mname], params.params[mname]);
    if (index >= 0) {
        savedVelocity[object] = flist->property(p_highlightMoveVelocity).toInt();
        flist->setProperty(p_highlightMoveVelocity, 99999);
        flist->setProperty(p_currentIndex, index);
        setText(QString(t_Name2DisplayField).arg(mname), datamodels[mname]->index(index).data(DOFPrintRole).toString());
    }
}

void DOFCalculator::setFontAndHeading(int fontsize) {
    engine->rootContext()->setContextProperty("globalFontSize", settings.values[s_fontSize]);
    if (QWindow *w = (QGuiApplication::topLevelWindows().size() > 0 ? QGuiApplication::topLevelWindows().at(0) : nullptr)) {
        int bigfont = (qMin(w->geometry().width(), w->geometry().height()) / qMax(1, fontsize) < 28);
        setText("hyperfocalPrefix", bigfont ? "HF:" : "HFocal:");
        setText("dofFarPrefix", bigfont ? "F:" : "Far:");
        setText("dofNearPrefix", bigfont ? "N:" : "Near:");
    }
}

void DOFCalculator::afterQMLStart(QGuiApplication* app) {
    connect(app, &QGuiApplication::applicationStateChanged, this, &DOFCalculator::appStateChanged);

    setFontAndHeading(settings.values[s_fontSize]);
    if (QObject *dialog = findObjectByName("settingsDialog"))
        connect(dialog, SIGNAL(settingsDialogChanged()), this, SLOT(settingsChanged()));

    foreach(const QString& mname, datamodels.keys()) {
        QString object = QString(t_Name2ListView).arg(mname);
        if (QObject *flist = findObjectByName(object)) {
            scrollers[mname] = flist;
            setViewIndexAndShow(mname, object, flist);
            connect(flist, SIGNAL(myViewIndexChanged(int)), this, SLOT(scrollerChanged(int)));
        }
    }
    foreach(const QString& key, settings.values.keys()) {
        QObject *obj = nullptr;
        if (key.startsWith(t_IsIndexVar) && (obj = findObjectByName(key.right(t_IndexNameLen))))
            obj->setProperty(p_currentIndex, settings.values[key]);
        else if ((obj = findObjectByName(key)))
            obj->setProperty(p_value, settings.values[key]);
    }
    calc();
}

int DOFCalculator::repopulate(const QString& mname) {
    if (!datamodels[mname] || !scrollers[mname]) return 0;
    QString object = QString(t_Name2ListView).arg(mname);
    if (QObject *flist = findObjectByName(object)) {
        scrollers[mname]->blockSignals(1);
        datamodels[mname]->populate(settings.values);
        setViewIndexAndShow(mname, object, flist);
        scrollers[mname]->blockSignals(0);
        return 1;
    }
    return 0;
}

void DOFCalculator::settingsChanged() {
    dofsettings_t osettings = settings;
    foreach(const QString& key, settings.values.keys()) {
        QObject *obj = nullptr;
        if (key.startsWith(t_IsIndexVar) && (obj = findObjectByName(key.right(t_IndexNameLen))))
            settings.values[key] = obj->property(p_currentIndex).toInt();
        else if ((obj = findObjectByName(key)))
            settings.values[key] = obj->property(p_value).toInt();
    }
    if (osettings.values == settings.values) return;
    settings.saveSettings();
    int repopulated = 0;
    if (settings.values[s_aperture] != osettings.values[s_aperture]) 	repopulated += repopulate(mv_aperture);
    if (settings.values[s_distance] != osettings.values[s_distance])	repopulated += repopulate(mv_distance);
    if (repopulated) QTimer::singleShot(2000, this, SLOT(restoreVelocities()));
    setFontAndHeading(settings.values[s_fontSize]);
    calc();
}

void DOFCalculator::scrollerChanged(int dindex) {
    dofparams_t oparams = params;
    QString key;
    foreach(const QString& mn, scrollers.keys())
        if (scrollers[mn] == sender())
            key = mn;
    if (key.length() && datamodels.contains(key) && dindex >= 0 && dindex < datamodels[key]->rowCount()) {
        params.params[key] = datamodels[key]->index(dindex).data(DOFValueRole).toReal();
        if (params.params == oparams.params) return;
        calc();
        setText(QString(t_Name2DisplayField).arg(key), datamodels[key]->index(dindex).data(DOFPrintRole).toString());
    }
}

int DOFCalculator::findNearestIndex(QAbstractListModel* model, qreal wanted) {
    if (model->rowCount() < 2) return model->rowCount() - 1;
    qreal current = model->index(0).data(DOFValueRole).toReal();
    qreal diff = qAbs(current - wanted);
    for (int i = 1; i < model->rowCount(); i++) {
        qreal val = model->index(i).data(DOFValueRole).toReal();
        if (diff < qAbs(val - wanted)) return i - 1;
        diff = qAbs(val - wanted);
    }
    return model->rowCount() - 1;
}

QObject* DOFCalculator::findObjectByName(const QString& object) {
    foreach(QObject* o, engine->rootObjects())
        if (QObject *item = o->findChild<QObject*>(object))
            return item;
    return nullptr;
}

void DOFCalculator::restoreVelocities() {
    foreach(const QString& oname, savedVelocity.keys())
        if (QObject * item = findObjectByName(oname))
            item->setProperty(p_highlightMoveVelocity, savedVelocity[oname]);
    savedVelocity.clear();
}

void DOFCalculator::calc() {
    qreal focalM = params.params[mv_focal] / 1000.0, CoCM = qreal(settings.values["coc"]) / 1000.0 / 1000.0, distance = params.params[mv_distance], aperture = params.params[mv_aperture];
    qreal airy = 0.38 * aperture / 1000.0 / 1000.0; // 0.38um - about shortest visible wavelength
    CoCM = qMax(CoCM, airy);
    qreal focalM2 = focalM * focalM;
    qreal H = focalM2 / aperture / CoCM + focalM;
    setText("hyperfocalString", distanceModel::prettyPrint(H));
    if (focalM > distance * 0.9)
    {
        setDOF(S_longdash, S_longdash);
        setText("deltaDisplay", " ");
    }
    else
    {
        qreal DOFNear = distance * focalM2 / (focalM2 - aperture * focalM * CoCM + aperture * distance * CoCM);
        qreal DOFFar = distance * focalM2 / (focalM2 + aperture * focalM * CoCM - aperture * distance * CoCM);

        if (DOFFar > 0)
        {
            qreal delta = (DOFFar - DOFNear) / DOFNear; 
            int digits = delta < 0.1 ? qBound(0, int(-1 * std::log10(delta))+1, 5) : -1;
            setDOF(distanceModel::prettyPrint(DOFNear,digits), distanceModel::prettyPrint(DOFFar,digits));
            setText("deltaDisplay", QString(" %1= %2").arg(QChar(0x394)).arg(distanceModel::prettyPrint(DOFFar - DOFNear)));
        }
        else
        {
            setDOF(distanceModel::prettyPrint(DOFNear), QString(QChar(0x221e)));
            setText("deltaDisplay", " ");
        }
    }
}
