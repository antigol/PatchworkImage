#include "patch.h"

Patch::Patch(const QString &fileName, const char *format) :
    QImage(fileName, format)
{
}

Patch::Patch(const char *fileName, const char *format) :
    QImage(fileName, format)
{
}

Patch::Patch(const QImage &image, QRgb avg) :
    QImage(image), _average(avg)
{
}

Patch::Patch(const Patch &copy) :
    QImage(copy), _average(copy._average)
{

}

QRgb Patch::average() const
{
    return _average;
}

void Patch::averaging()
{
    _average = QImage::scaled(1, 1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).pixel(0, 0);
}

QImage Patch::scaled(const QSize &s)
{
    if (_temporary.size() != s) {
        _temporary = QImage::scaled(s, Qt::KeepAspectRatioByExpanding);
    }
    return _temporary;
}
