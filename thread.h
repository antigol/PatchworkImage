#ifndef THREAD_H
#define THREAD_H

#include "patch.h"

#include <QThread>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = 0);

    void addPatch(Patch *ptr);
    int sizeList() const;

signals:
    void onemore(int);
    void finished(bool);

public slots:

private:
    void run();

    QList<Patch *> _patches;
};

#endif // THREAD_H
