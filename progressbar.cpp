#include "progressbar.h"

ProgressBar::ProgressBar(QWidget *parent) :
    QProgressBar(parent)
{
}

void ProgressBar::increment(int x)
{
    setValue(value() + x);
}
