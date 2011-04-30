#include "compareaverage.h"

CompareAverage::CompareAverage(const QImage &workScaledMini, const QList<Patch *> &patches, int beg, int end, ScaleAndPaste *sap, QObject *parent) :
    QThread(parent), _work(workScaledMini), _patches(patches), _beg(beg), _end(end), _sap(sap)
{
}

void CompareAverage::run()
{
    for (int i = _beg; i < _end; ++i) {
        const int x = i % _work.width();
        const int y = i / _work.width();

        const QRgb a = _work.pixel(x, y);
        const int ar = qRed(a);
        const int ag = qGreen(a);
        const int ab = qBlue(a);
        const int agray = qGray(a);

        int min = 1000;
        int id = -1;
        for (int j = 0; j < _patches.size(); ++j) {

            int delta = qAbs(ar - qRed(_patches[j]->average())) +
                    qAbs(ag - qGreen(_patches[j]->average())) +
                    qAbs(ab - qBlue(_patches[j]->average())) +
                    qAbs(agray - qGray(_patches[j]->average()));

            if (delta < min) {
                min = delta;
                id = j;
            }
        }

        _sap->paste(id, QPoint(x, y));
    }
}
