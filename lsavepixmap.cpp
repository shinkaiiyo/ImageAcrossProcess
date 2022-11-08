#include "lsavepixmap.h"
#include <thread>
#include <QList>
#include <QBuffer>
#include <QDebug>
#include <QImageReader>

QSystemSemaphore LSavePixMap::m_lockSrc(QString("lockSrc"), 1, QSystemSemaphore::Create);
QSystemSemaphore LSavePixMap::m_lockDst(QString("lockDst"), 0, QSystemSemaphore::Create);

LSavePixMap::LSavePixMap(int a, int b, int c, int d)
{
    x = a;
    y = b;
    l = c;
    h = d;
    m_mem.setKey(QString("sharedMem"));
    if (m_mem.isAttached())
    {
        m_mem.detach();
    }
    m_mem.create(10000000);
}

LSavePixMap::~LSavePixMap()
{

}

void LSavePixMap::slotSavePixMap(int i, QImage img)
{
    image = QPixmap::fromImage(img.copy(x - 10, y - 100, l, h));
    writeData(image);
}

void LSavePixMap::writeData(const QPixmap &img)
{
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    img.save(&buffer, "png");
    int size = buffer.size();
    QImage image = QImageReader(&buffer, "png").read();
    QPixmap img1 = QPixmap::fromImage(image);
    QString locate = "D:/" + QString::number(i) + ".png";
    img1.save(locate);

    if(m_mem.size() < size)
    {
        qDebug() << "not enough mem size";
        return;
    }

    if(m_lockSrc.acquire())
    {
        // Write into the shared memory
        m_mem.lock();
        char *to = (char*)m_mem.data();
        const char *from = buffer.data().data();
        memcpy( to, from, qMin( m_mem.size(), size));
        m_mem.unlock();
        m_lockDst.release();
    }
}

void LSavePixMap::testWrite(const QString &string)
{
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << string;
    int size = buffer.size();

    if(m_mem.size() < size)
    {
        qDebug() << "not enough mem size";
        return;
    }

    if(m_lockSrc.acquire())
    {
        // Write into the shared memory
        m_mem.lock();
        char *to = (char*)m_mem.data();
        const char *from = buffer.data().data();
        memcpy( to, from, qMin( m_mem.size(), size ) );
        m_mem.unlock();
        m_lockDst.release();
    }
}
