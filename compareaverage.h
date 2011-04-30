#ifndef COMPAREAVERAGE_H
#define COMPAREAVERAGE_H

#include "patch.h"
#include "scaleandpaste.h"

#include <QThread>
#include <QImage>

class CompareAverage : public QThread
{
    Q_OBJECT
public:
    CompareAverage(const QImage &workScaledMini, const QList<Patch *> &patches, int beg, int end, QSize patchSize, ScaleAndPaste *sap, QObject *parent = 0);
    ~CompareAverage();

public slots:

private:
    void run();

    bool _stop;
    QImage _work;
    QList<Patch *> _patches;
    int _beg, _end;
    QSize _patchSize;
    ScaleAndPaste *_sap;
};

#endif // COMPAREAVERAGE_H
