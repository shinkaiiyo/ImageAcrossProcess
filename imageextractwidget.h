#ifndef ImageExtractWidget_H
#define ImageExtractWidget_H
#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QImage>
#include <QPixmap>
#include <QTimer>

class LSavePixMap;
class QLabel;
class QPushButton;
class QCamera;
class QCameraImageCapture;
class QTimer;
class QCameraViewfinder;
class ImageExtractWidget : public QWidget
{
    Q_OBJECT
public:
    ImageExtractWidget(QWidget *parent);
    ~ImageExtractWidget();
    bool ifConnect();

public slots:
    void slotStartChoose();
    void slotFinishChoose();

private:
    void layoutUI();
    void connectFunc();

private:
    QLabel *backgroundLabel;
    QLabel *maskLabel;
    QPushButton *startButton;
    QPushButton *finishButton;
    bool ifChoosing;
    QPoint peakPoint;
    int lenth;
    int hight;
    QCamera *camera;
    QCameraImageCapture *camera_image_capture;
    QCameraViewfinder *viewfind;
    LSavePixMap *savePixMap;
    QTimer *saveTimer;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

public slots:
    void slotSaveImage();

};

#endif // ImageExtractWidget_H
