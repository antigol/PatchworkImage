#ifndef WIDGET_H
#define WIDGET_H

#include "patch.h"
#include "thread.h"
#include "scaleandpaste.h"
#include "compareaverage.h"

#include <QWidget>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void changeEvent(QEvent *e);

private:

private slots:
    void appendPatches();
    void loadImage();
    void patch();
    void patchfinished();

private:
    void loadSettings();
    void saveSettings();

    Ui::Widget *ui;
    QImage workingImage;
    QList<Patch *> _patches;

    QList<Thread *> _averagePatches;

    ScaleAndPaste *_scaleAndPaste;
    QList<CompareAverage *> _compareAverages;
};

int cpucount();

#endif // WIDGET_H
