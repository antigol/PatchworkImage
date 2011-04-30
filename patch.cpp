#include "patch.h"

Patch::Patch(const QString &fileName) :
	_filename(fileName)
{
}

void Patch::create()
{
	_original.load(_filename);
}

QRgb Patch::average() const
{
    return _average;
}

void Patch::averaging()
{
	_average = _original.scaled(1, 1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).pixel(0, 0);
}

QImage Patch::scaled(const QSize &s)
{
    if (_temporary.size() != s) {
		_temporary = _original.scaled(s, Qt::KeepAspectRatioByExpanding);
    }
    return _temporary;
}
