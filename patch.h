#ifndef PATCH_H
#define PATCH_H

#include <QImage>
#include <QColor>

class Patch : public QImage
{
public:
    Patch(const QString &fileName, const char *format = 0);
    Patch(const char *fileName, const char *format = 0);
    Patch(const QImage &image, QRgb avg = 0);
    Patch(const Patch &copy);

    QRgb average() const;
    void averaging();

    QImage scaled(const QSize &s);

private:
    QRgb _average;

    QImage _temporary;
};

#endif // PATCH_H
