#ifndef WIDGET_H
#define WIDGET_H

#include "patch.h"
#include "thread.h"

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

private:
    Ui::Widget *ui;
    QImage workingImage;
    QList<Patch *> _patches;
    QList<Thread *> _threads;
};

#endif // WIDGET_H
