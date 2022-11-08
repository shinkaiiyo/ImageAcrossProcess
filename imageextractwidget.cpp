#include "imageextractwidget.h"
#include <iostream>
#include <vector>
#include <QLabel>
#include <QPushButton>
#include <QMetaType>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QList>
#include <QTimer>
#include "lsavepixmap.h"
#pragma execution_character_set("utf-8")

ImageExtractWidget::ImageExtractWidget(QWidget *parent) : QWidget(parent), backgroundLabel(NULL),
    maskLabel(NULL), startButton(NULL), finishButton(NULL), ifChoosing(false), lenth(0),
    hight(0), camera(NULL), camera_image_capture(NULL), viewfind(NULL), savePixMap(NULL), saveTimer(NULL)
{
    layoutUI();
    connectFunc();
}

ImageExtractWidget::~ImageExtractWidget()
{
    if(camera)
    {
        camera->stop();
        delete camera;
        camera = NULL;
    }
    if(viewfind)
    {
        delete viewfind;
        viewfind = NULL;
    }
    if(camera_image_capture)
    {
        delete camera_image_capture;
        camera_image_capture = NULL;
    }
}

bool ImageExtractWidget::ifConnect()
{
    if(QCamera::State::UnloadedState == camera->state())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void ImageExtractWidget::mousePressEvent(QMouseEvent *e)
{
    if(Qt::MouseButton::LeftButton == e->button())
    {
        QPoint point = e->pos();
        if(ifChoosing && point.x() > 10 && point.x() < 1910 && point.y() > 100 && point.y() < 2000)
        {
            maskLabel->setVisible(true);
            peakPoint = point;
            maskLabel->move(peakPoint);
        }
    }
}

void ImageExtractWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(Qt::MouseButton::LeftButton == e->button())
    {
        QPoint point = e->pos();
        if(ifChoosing && point.x() > 10 && point.x() < 1910 && point.y() > 100 && point.y() < 2000)
        {
            if(maskLabel->isVisible())
            {
                ifChoosing = false;
                if(point.x() < peakPoint.x())
                {
                    if(point.y() < peakPoint.y())
                    {
                        peakPoint.setX(point.x());
                        peakPoint.setY(point.y());
                    }
                    else
                    {
                        peakPoint.setX(point.x());
                    }
                }
                else
                {
                    if(point.y() < peakPoint.y())
                    {
                        peakPoint.setY(point.y());
                    }
                }
            }
        }
    }
}

void ImageExtractWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint point = e->pos();
    if(ifChoosing && point.x() > 10 && point.x() < 1910 && point.y() > 100 && point.y() < 2000)
    {
        lenth = std::abs(point.x() - peakPoint.x());
        hight = std::abs(point.y() - peakPoint.y());
        maskLabel->setFixedSize(lenth, hight);
        if(point.x() < peakPoint.x())
        {
            if(point.y() < peakPoint.y())
            {
                maskLabel->move(point.x(), point.y());
            }
            else
            {
                maskLabel->move(point.x(), peakPoint.y());
            }
        }
        else
        {
            if(point.y() < peakPoint.y())
            {
                maskLabel->move(peakPoint.x(), point.y());
            }
            else
            {
                maskLabel->move(peakPoint.x(), peakPoint.y());
            }
        }
        update();
    }
}

void ImageExtractWidget::slotStartChoose()
{
    ifChoosing = true;
    maskLabel->setVisible(false);
}

void ImageExtractWidget::slotFinishChoose()
{
    if(savePixMap)
    {
        delete savePixMap;
        savePixMap = NULL;
        savePixMap = new LSavePixMap(peakPoint.x(), peakPoint.y(), lenth, hight);
    }
    else
    {
        savePixMap = new LSavePixMap(peakPoint.x(), peakPoint.y(), lenth, hight);
        connect(camera_image_capture, SIGNAL(imageCaptured(int, QImage)), savePixMap, SLOT(slotSavePixMap(int, QImage)));
    }
    if(!saveTimer)
    {
        saveTimer = new QTimer(this);
        saveTimer->setSingleShot(false);
        connect(saveTimer, SIGNAL(timeout()), this, SLOT(slotSaveImage()));
        saveTimer->start(100);
    }
}

void ImageExtractWidget::layoutUI()
{
    setFixedSize(1920, 2160);
    backgroundLabel = new QLabel(this);
    viewfind = new QCameraViewfinder(this);//打开取景器
    maskLabel = new QLabel(this);
    startButton = new QPushButton(this);
    finishButton = new QPushButton(this);
    backgroundLabel->setGeometry(0, 0, 1920, 2160);
    backgroundLabel->setStyleSheet("QLabel{"
                                   "background-color: rgb(23, 27, 38);"
                                   "}");
    maskLabel->setGeometry(0, 0, 0, 0);
    maskLabel->setStyleSheet("QLabel{"
                             "background-color: rgba(169, 169, 169, 50%);"
                             "}");
    maskLabel->setVisible(false);
    startButton->setGeometry(760, 2050, 150, 50);
    startButton->setText("开始选择");
    startButton->setStyleSheet("QPushButton{"
                               "font-size:20px;"
                               "background-color: rgb(0, 178, 84);"
                               "border-radius:8px;"
                               "color:white;"
                               "}"
                               "QPushButton::pressed{"
                               "padding-left:5px;"
                               "padding-top:5px;"
                               "}"
                               "QPushButton::disabled{"
                               "background-color: rgb(182, 193, 205);"
                               "border:0px groove gray;"
                               "border-radius:5px;"
                               "color: rgb(153, 153, 153);"
                               "}"
                               ""
                               "QPushButton:focus{outline: none;}");
    finishButton->setGeometry(1010, 2050, 150, 50);
    finishButton->setText("完成选择");
    finishButton->setStyleSheet("QPushButton{"
                                "font-size:20px;"
                                "background-color: rgb(0, 178, 84);"
                                "border-radius:8px;"
                                "color:white;"
                                "}"
                                "QPushButton::pressed{"
                                "padding-left:5px;"
                                "padding-top:5px;"
                                "}"
                                "QPushButton::disabled{"
                                "background-color: rgb(182, 193, 205);"
                                "border:0px groove gray;"
                                "border-radius:5px;"
                                "color: rgb(153, 153, 153);"
                                "}"
                                ""
                                "QPushButton:focus{outline: none;}");
    setMouseTracking(true);

    QList<QCameraInfo> list;
    list = QCameraInfo::availableCameras();
    camera = new QCamera(list.at(0));//设定摄像头
    camera_image_capture = new QCameraImageCapture(camera);
    camera_image_capture->setCaptureDestination(QCameraImageCapture::CaptureToFile);

    camera->setCaptureMode(QCamera::CaptureVideo);
    viewfind->setGeometry(10, 100, 1900, 1900);
    camera->setViewfinder(viewfind);//相机匹配取景器
    camera->start();//启动摄像头
}

void ImageExtractWidget::connectFunc()
{
    connect(startButton, SIGNAL(clicked()), this, SLOT(slotStartChoose()));
    connect(finishButton, SIGNAL(clicked()), this, SLOT(slotFinishChoose()));
}

void ImageExtractWidget::slotSaveImage()
{
    camera_image_capture->capture();
}
