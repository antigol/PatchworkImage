#ifndef PATCH_H
#define PATCH_H

#include <QImage>
#include <QColor>

class Patch
{
public:
	Patch(const QString &fileName);

	void create();

    QRgb average() const;
    void averaging();

    QImage scaled(const QSize &s);

private:
    QRgb _average;

	QString _filename;

	QImage _original;
    QImage _temporary;
};

#endif // PATCH_H
