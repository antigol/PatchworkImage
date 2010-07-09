#ifndef PATCH_H
#define PATCH_H

#include <QImage>
#include <QColor>

class Patch : public QImage
{
public:
    Patch(const QString &fileName, const char *format = 0);
    Patch(const char *fileName, const char *format = 0);
    Patch(const QImage &image);

    const QColor &average() const;

public:
    void averaging();

private:
    QColor _average;
};

#endif // PATCH_H
