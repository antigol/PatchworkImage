#include "compareaverage.h"

CompareAverage::CompareAverage(const QImage &workScaledMini, const QList<Patch *> &patches, int beg, int end, QSize patchSize, ScaleAndPaste *sap, QObject *parent) :
    QThread(parent), _work(workScaledMini), _patches(patches), _beg(beg), _end(end), _patchSize(patchSize), _sap(sap)
{
}

CompareAverage::~CompareAverage()
{
    _stop = true;
    wait();
}

void CompareAverage::run()
{
    _stop = false;

    for (int i = _beg; i < _end && !_stop; ++i) {
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

        _patches[id]->scaled(_patchSize);
        _sap->paste(id, QPoint(x, y));
    }
}
