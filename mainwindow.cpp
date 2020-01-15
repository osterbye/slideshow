/*
MIT License

Copyright (c) 2020 Nikolaj Due Osterbye

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTimer>


#ifndef SD_MOUNT_LOCATION
#define SD_MOUNT_LOCATION   "/tmp/SD0"
#endif
#define CONTENT_DIR         SD_MOUNT_LOCATION "/slideshow/"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_imageTimer = new QTimer();
    m_imageTimer->setSingleShot(true);
    connect(m_imageTimer, SIGNAL(timeout()), this, SLOT(updateImage()));
    m_imageTimer->start(getInterval());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage()
{
    ui->labelImage->setPixmap(getNextImage());
    update();
    m_imageTimer->start(getInterval());
}

/*
 * Returns the interval, in milliseconds, for next image change.
 * Return value read from interval text file or 1000 in case text file is not
 * present or unreadable
*/
int MainWindow::getInterval()
{
    int ret = 1000;

    QFile intervalFile(CONTENT_DIR "interval.txt");
    if (intervalFile.exists()) {
        if (intervalFile.open(QIODevice::ReadOnly)) {
            QString intervalString = intervalFile.readAll();
            int interval = intervalString.toInt();
            intervalFile.close();
            if (interval > 0)
                ret = interval;
        }
    }

    return ret;
}

/*
 * Returns the next image to be shown
*/
QPixmap MainWindow::getNextImage()
{
    QDir imageDirectory(CONTENT_DIR);
    QStringList filters;
    filters << "*.png" << "*.jpg";
    imageDirectory.setNameFilters(filters);

    QStringList images  = imageDirectory.entryList();
    int imageCount = images.count();
    if (imageCount > 0) {
        if (++m_imageIndex >= imageCount)
            m_imageIndex = 0;

        QPixmap image;
        if (image.load(CONTENT_DIR + images.at(m_imageIndex)))
            return image;

    }

    return QPixmap(":/logo_p.png");
}
