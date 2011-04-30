#ifndef SCALEANDPASTE_H
#define SCALEANDPASTE_H

#include "patch.h"

#include <QThread>
#include <QImage>
#include <QPair>
#include <QMutex>

class ScaleAndPaste : public QThread
{
    Q_OBJECT
public:
    ScaleAndPaste(const QList<Patch *> &patches, QSize workSize, QSize patchSize, QObject *parent = 0);
    QImage finalImage() const;

signals:
    void progress(int);

public slots:
    void paste(int patchid, QPoint pos);
    void stop();

private:
    void run();
    void addPatch(const QImage &pa, QPoint pos);

    QList<Patch *> _patches;
    QSize _workSize;
    QSize _patchSize;

    QImage _work;
    QList<QPair<int, QPoint> > _todo;
    QMutex _mx;
    bool _stop;
};

#endif // SCALEANDPASTE_H
