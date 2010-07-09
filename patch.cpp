#include "patch.h"

Patch::Patch(const QString &fileName, const char *format) :
        QImage(fileName, format)
{
}

Patch::Patch(const char *fileName, const char *format) :
        QImage(fileName, format)
{
}

Patch::Patch(const QImage &image) :
        QImage(image)
{
}

const QColor &Patch::average() const
{
    return _average;
}

void Patch::averaging()
{
    QRgb p;
    uint r = 0, g = 0, b = 0;

    for (int x = 0; x < width(); ++x)
        for (int y = 0; y < height(); ++y) {
        p = pixel(x, y);
        r += qRed(p);
        g += qGreen(p);
        b += qBlue(p);
    }

    uint size = width() * height();

    r /= size;
    g /= size;
    b /= size;

    _average.setRgb(r, g, b);
}
