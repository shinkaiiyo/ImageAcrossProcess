#ifndef LSAVEPIXMAP_H
#define LSAVEPIXMAP_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QSharedMemory>
#include <QSystemSemaphore>
class LSavePixMap : public QObject
{
    Q_OBJECT
public:
    LSavePixMap(int a, int b, int c, int d);
    ~LSavePixMap();

private:
    void writeData(const QPixmap &img);
    void testWrite(const QString &string);

public slots:
    void slotSavePixMap(int i,QImage img);

private:
    QPixmap image;
    QString path;
    int x, y, l, h;
    QSharedMemory m_mem;
    static QSystemSemaphore            m_lockSrc;
    static QSystemSemaphore            m_lockDst;
    int i = 0;

signals:
    void signalFinishCap(const QPixmap &);
};

#endif // LSAVEPIXMAP_H
