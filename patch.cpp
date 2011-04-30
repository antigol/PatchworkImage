#include "patch.h"

Patch::Patch(const QString &fileName) :
    _filename(fileName)
{
}

void Patch::create()
{
    _average = QImage(_filename).scaled(1, 1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).pixel(0, 0);
}

QRgb Patch::average() const
{
    return _average;
}

const QImage &Patch::scaled(const QSize &s)
{
    _mx.lock();
    if (_temporary.size() != s) {
        _temporary = QImage(_filename).scaled(s, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        int x = (_temporary.width() - s.width()) / 2;
        int y = (_temporary.height() - s.height()) / 2;
        _temporary = _temporary.copy(x, y, s.width(), s.height());
    }
    _mx.unlock();
    return _temporary;
}
