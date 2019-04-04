/*  DOFCalc v0.1
   (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
   The program is licensed under the terms of GNU General Public License v3 */

#include "datamodels.h"
#include <QSet>
#include <QRegularExpression>

QVariant focalModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const {
    int row = index.row();
    if (row < 0 || row >= rowCount()) return QVariant();
    switch (role) {
    case DOFDisplayRole: if (!datalist[row].big) return S_longdash; // fall-through
    case DOFPrintRole: 	return formatString.arg(datalist[row].value, 0, 'f', digits(datalist[row].value));
    case DOFValueRole:  return datalist[row].value;
    case DOFSizeRole:   return int(datalist[row].big);
    }
    return QVariant();
}

void focalModel::populate0(const QHash<QString, int>&) {
    struct focal_params_t { qreal begin, step; int div; };
    QList<focal_params_t> focal_params(QList<focal_params_t>{ {5, 1, 5}, { 30,5,10, }, { 150,10,50 }, { 250,25,50 }, { 400,50,100 }, { 1000,100,200 }, { 1600,0,100 } });
    for (int i = 0; i < focal_params.size(); i++) {
        datalist.push_back(floatitem_t(focal_params[i].begin, true));
        if (focal_params[i].step < 0.001) break;
        qreal start = focal_params[i].begin + focal_params[i].step;
        while (start < focal_params[i + 1].begin) {
            datalist.push_back(floatitem_t(start, !(int(start) % focal_params[i].div)));
            start += focal_params[i].step;
        }
    }
}

void apertureModel::populate0(const QHash<QString, int>& params) {
    QList<qreal>fstoplist[4] = { // log2 based calculations do not match widely used aperture scales; 1/2 and 1/3 scales are copied from Seconic L558 scale; 1/4 scale matches seconic 1/2
        {QList<qreal>{0.7, 1, 1.4, 2, 2.8, 4, 5.6, 8, 11, 16, 22, 32, 45, 64, 90, 128, 180, 256}},
        {QList<qreal>{0.7, 0.8, 1, 1.2,1.4,1.7, 2,2.4, 2.8, 3.4, 4, 4.8, 5.6, 6.7, 8, 9.5, 11,13, 16,19, 22,27, 32,38, 45,54, 64, 76, 90, 108, 128, 152, 180, 215, 256}},
        {QList<qreal>{0.7, 0.8, 0.9, 1, 1.1,1.3, 1.4, 1.6, 1.8, 2, 2.2, 2.5, 2.8, 3.2, 3.6, 4, 4.5, 5, 5.6, 6.3, 7.1, 8, 9, 10, 11, 13,14, 16, 18, 20, 22, 25, 29, 32,
                    36, 40, 45, 51, 57, 64,72, 81, 90, 102,114, 128,144, 161, 180,200,225,256 }},
        {QList<qreal>{0.7,0.75, 0.8,0.9, 1, 1.1, 1.2, 1.3,1.4, 1.5, 1.7,1.8, 2,2.2, 2.4,2.6, 2.8, 3.1, 3.4, 3.7, 4,4.4,4.8, 5.2, 5.6,6.2, 6.7,7.3, 8,8.7, 9.5,10, 11,12,13,
                    15,16,17,19,21,22,25,27,29,32,34,38,42,45,50,54,58,64,68,76,84,90,100,108,116,128, 135, 152, 165, 180, 200, 215, 230, 256}},
    };

    int ns = qBound(0, params[s_aperture], 3);
    for (int i = 0; i < fstoplist[ns].size(); i++)
        datalist.push_back(floatitem_t(fstoplist[ns][i], !(i % (ns + 1))));
}

static QString cutzeroes(qreal v, int digits) {
    QString s = QString("%1").arg(v, 0, 'f', digits);
    if (s.contains('.'))
        s.replace(QRegularExpression("\\.0+$"), "");
    if (s.contains('.'))
        s.replace(QRegularExpression("0+$"), "");
    return s;
}

QString distanceModel::prettyPrint(qreal val)
{
    if (std::abs(val) < 0.001) // micrometers
        return QString("%1 %2m").arg(cutzeroes(val*1000000.0,_digits(val*10000000.0))).arg(QChar(0x3bc));
    else if (std::abs(val) < 0.01) // less than 1cm -> millimeters
        return QString("%1 mm").arg(cutzeroes(val*1000.0, _digits(val*1000.0)));
    else if (std::abs(val) < 1) // less than 1m -> centimeters
        return QString("%1 cm").arg(cutzeroes(val*100.0, _digits(val*100.0)));
    else if (std::abs(val) >= 1000) // km
        return QString("%1 km").arg(cutzeroes(val/1000.0, _digits(val/10000.0)));
    return QString("%1 m").arg(val, 0, 'f', 2);
}

QVariant distanceModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const {
    if (index.row() >= 0 && index.row() < rowCount())
        if ((role == DOFDisplayRole && datalist[index.row()].big) || role == DOFPrintRole)
            return prettyPrint(datalist[index.row()].value);
    return focalModel::data(index, role);
}

void distanceModel::populate0(const QHash<QString, int>& settings) {
    QList<qreal> l10 = QList<qreal>{ 0.1, 0.125, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5, 0.6, 0.75, 1 }; // Again: manual distance list is much better than generated one
    QSet<int> B = QSet<int>{ 0, 3, 5, 7 };
    int f1 = qBound(1, settings[s_distance] + 1, 3);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < l10.size() - 1; j++) {
            datalist.push_back(floatitem_t(std::pow(10.0, qreal(i))*l10[j], f1 > 1 || B.contains(j)));
            for (int k = 1; k < f1; k++)
                datalist.push_back(floatitem_t(l10[j] * std::exp(std::log(l10[j + 1] / l10[j]) / qreal(f1)*k)*std::pow(10.0, qreal(i)), false));
        }
    datalist.push_back(floatitem_t(1000, true));
}
