#include "thread.h"

Thread::Thread(QObject *parent) :
    QThread(parent)
{
}

void Thread::addPatch(Patch *ptr)
{
    _patches.append(ptr);
}

int Thread::sizeList() const
{
    return _patches.size();
}

void Thread::run()
{
    while (!_patches.isEmpty()) {
        Patch *p = _patches.takeFirst();
        p->create();
        emit onemore(1);
    }
    emit finished(true);
}
