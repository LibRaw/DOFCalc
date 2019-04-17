/* DOFCalc v0.1
    (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
    The program is licensed under the terms of GNU General Public License v3 */

#pragma once
#include <QAbstractListModel>
#include "dofstrings.h"
#include <cmath>

enum DOFRoles { DOFDisplayRole = Qt::UserRole + 1, DOFValueRole, DOFSizeRole, DOFPrintRole };

struct floatitem_t {
    qreal value;
    bool  big;
    floatitem_t() : value(1), big(false) {}
    floatitem_t(qreal v, bool b) : value(v), big(b) {}
    floatitem_t(const floatitem_t& s) : value(s.value), big(s.big) {}
};

class focalModel : public QAbstractListModel {
    Q_OBJECT
public:
    focalModel(QObject *parent = nullptr) : QAbstractListModel(parent), formatString(f_focal) { }
    int rowCount(const QModelIndex &parent = QModelIndex()) const { Q_UNUSED(parent); return datalist.size(); }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const {
        return QHash<int, QByteArray>{ {DOFDisplayRole, "displayname"}, { DOFValueRole, "value" }, { DOFSizeRole, "rowsize" }};
    }
    void populate(const QHash<QString, int>& s) { beginResetModel(); datalist.clear(); populate0(s); endResetModel(); }
protected:
    virtual void populate0(const QHash<QString, int>& s);
    virtual int digits(qreal) const { return 0; }
    QString formatString;
    QList<floatitem_t> datalist;
};

class apertureModel : public focalModel {
    Q_OBJECT
public:
    apertureModel(QObject *parent = nullptr) : focalModel(parent) { formatString = QString(f_aperture); }
protected:
    virtual void populate0(const QHash<QString, int>&);
    virtual int digits(qreal val) const {
        if (val > 12) return 0;
        else if (val >= 1) return (std::abs(val - std::round(val)) < 0.1 ? 0 : 1);
        else  return (std::abs(val * 10 - std::round(val * 10)) < 0.5 ? 1 : 2);
    }
};

class distanceModel : public focalModel {
    Q_OBJECT
public:
    distanceModel(QObject *parent = nullptr) : focalModel(parent) {}
    virtual void populate0(const QHash<QString, int>&);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    static QString prettyPrint(qreal val, int digits = -1);
protected:
    static int _digits(qreal val) { return int(qMax(0.0, std::round(2.0 - std::log10(val)))); }
};
