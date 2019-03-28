/* DOFCalc v0.1
   (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
   The program is licensed under the terms of GNU General Public License v3 */

#pragma once
#include <QQmlApplicationEngine>
#include <QSettings>
#include "datamodels.h"
#include <QTimer>
#include <QGuiApplication>

struct dofsettings_t {
    QHash<QString, int> values;
    dofsettings_t() : values(QHash<QString, int>{ {"coc", 25}, { s_fontSize,18 }, { s_aperture,2 }, { s_distance ,1 }}) {}
    dofsettings_t(const dofsettings_t& s) : values(s.values) {}
    QSettings settings;
    void loadSettings(int fonts) {
        foreach(const QString& key, values.keys())
            if (settings.contains(key)) values[key] = settings.value(key).toInt();
            else if (key == QString(s_fontSize)) values[key] = fonts;
    }
    void saveSettings() { foreach(const QString& key, values.keys()) settings.setValue(key, values[key]); settings.sync(); }
};

struct dofparams_t {
    QHash<QString, qreal> params;
    dofparams_t() : params(QHash<QString, qreal>{ {mv_focal, 50}, { mv_aperture,5.6 }, { mv_distance,10.0 }}) {}
    dofparams_t(const dofparams_t& s) : params(s.params) {}
    QSettings settings;
    void loadSettings() { foreach(const QString& key, params.keys())if (settings.contains(key)) params[key] = settings.value(key).toReal(); }
    void saveSettings() { foreach(const QString& key, params.keys()) settings.setValue(key, params[key]); settings.sync(); }
};

class DOFCalculator : public QObject {
    Q_OBJECT
public:
    DOFCalculator(QQmlApplicationEngine *e, QObject* parent = nullptr);
    void afterQMLStart(QGuiApplication *app);
    ~DOFCalculator() { params.saveSettings(); }
public slots:
    void scrollerChanged(int);
    void restoreVelocities();
    void settingsChanged();
    void appStateChanged(Qt::ApplicationState s) { if (s == Qt::ApplicationSuspended) params.saveSettings(); }
private:
    int repopulate(const QString& mname);
    void setFontAndHeading(int fontsize);
    void setViewIndexAndShow(const QString &mname, const QString& object, QObject*);
    void calc();
    void setText(const QString& oname, const QString& text) { if (QObject *textObj = findObjectByName(oname)) textObj->setProperty("text", text); }
    void setDOF(const QString& near, const QString& fars) { setText("dofNearString", near);	setText("dofFarString", fars); }
    QObject* findObjectByName(const QString& object);
    int  findNearestIndex(QAbstractListModel* model, qreal wanted);
    QQmlApplicationEngine *engine;
    dofparams_t params;
    dofsettings_t settings;
    QHash<QString, focalModel*> datamodels;
    QHash<QString, QObject*> scrollers;
    QHash<QString, int> savedVelocity;
};
