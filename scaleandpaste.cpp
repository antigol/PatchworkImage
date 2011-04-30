#include "scaleandpaste.h"
#include <QMutexLocker>

ScaleAndPaste::ScaleAndPaste(const QList<Patch *> &patches, QSize workSize, QSize patchSize, QObject *parent) :
    QThread(parent), _patches(patches), _workSize(workSize), _patchSize(patchSize),
    _work(QSize(workSize.width() * patchSize.width(), workSize.height() * patchSize.height()), QImage::Format_RGB32)
{
}

QImage ScaleAndPaste::finalImage() const
{
    return _work;
}

void ScaleAndPaste::stop()
{
    _stop = true;
}

void ScaleAndPaste::paste(int patchid, QPoint pos)
{
    QMutexLocker locker(&_mx);
    _todo.append(QPair<int, QPoint>(patchid, pos));
}

void ScaleAndPaste::run()
{
    QPair<int, QPoint> job;
    int paste = 0;
    int total = _workSize.width() * _workSize.height();
    _stop = false;

    while (paste < total && !_stop) {
        while (!_todo.isEmpty()) {
            _mx.lock();
            job = _todo.takeLast();
            _mx.unlock();

            // scale & paste
            addPatch(_patches[job.first]->scaled(_patchSize), job.second);

            ++paste;
            emit progress(paste);
            qDebug("(%d/%d)", paste, total);
        }
        msleep(20);
    }
}

void ScaleAndPaste::addPatch(const QImage &pa, QPoint pos)
{
    pos.setX(pos.x() * _patchSize.width());
    pos.setY(pos.y() * _patchSize.height());

    for (int x = 0; x < _patchSize.width(); ++x)
        for (int y = 0; y < _patchSize.height(); ++y) {
            _work.setPixel(pos.x() + x, pos.y() + y, pa.pixel(x, y));
        }
}
