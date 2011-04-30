#ifndef PATCH_H
#define PATCH_H

#include <QImage>
#include <QColor>
#include <QMutex>

class Patch
{
public:
    Patch(const QString &fileName);

    void create();

    QRgb average() const;

    const QImage &scaled(const QSize &s);

private:
    QRgb _average;

    QString _filename;

    QImage _temporary;

    QMutex _mx;
};

#endif // PATCH_H
