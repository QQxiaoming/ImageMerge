#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filedialog.h"
#include <QImage>
#include <QPixmap>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->horizontalSlider->setRange(0, 1023);
    ui->horizontalSlider->setValue(512);
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::on_horizontalSlider_valueChanged);
}

void MainWindow::setOffset(int offset) {
    m_offset = offset;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButtonOpen_clicked() {
    QString fileName = FileDialog::getOpenFileName(this, "Open Image", QString(), "Image Files (*.png *.PNG *.jpg *.JPG *.jpeg *.JPEG)");
    if (fileName.isEmpty())
        return;

    QPixmap pixmap;
    if (!pixmap.load(fileName)) {
        return;
    }

    ui->labelImg->setPixmap(pixmap);
}

void MainWindow::on_pushButtonOpen2_clicked() {
    QString fileName = FileDialog::getOpenFileName(this, "Open Image", QString(), "Image Files (*.png *.PNG *.jpg *.JPG *.jpeg *.JPEG)");
    if (fileName.isEmpty())
        return;

    QPixmap pixmap;
    if (!pixmap.load(fileName)) {
        return;
    }

    ui->labelImg2->setPixmap(pixmap);
}

void MainWindow::on_pushButtonMerge_clicked() {
    QPixmap pix1 = ui->labelImg->pixmap();
    QPixmap pix2 = ui->labelImg2->pixmap();

    if (pix1.isNull() || pix2.isNull()) {
        return;
    }

    QImage img1 = pix1.toImage().convertToFormat(QImage::Format_RGB32);
    QImage img2 = pix2.toImage().convertToFormat(QImage::Format_RGB32);

    // resize to the same size (use max dimensions)
    int w = qMax(img1.width(), img2.width());
    int h = qMax(img1.height(), img2.height());
    if (img1.size() != QSize(w, h)) img1 = img1.scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    if (img2.size() != QSize(w, h)) img2 = img2.scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    // Preprocess: compress ranges per channel
    // img1: map [0,255] -> [offset,255]
    // img2: map [0,255] -> [0,offset]
    int off = offset();
    if (off < 0) off = 0;
    if (off > 255) off = 255;
    for (int y = 0; y < h; ++y) {
        QRgb *line1 = reinterpret_cast<QRgb*>(img1.scanLine(y));
        QRgb *line2 = reinterpret_cast<QRgb*>(img2.scanLine(y));
        for (int x = 0; x < w; ++x) {
            QRgb p1 = line1[x];
            QRgb p2 = line2[x];

            int r1 = qRed(p1);
            int g1 = qGreen(p1);
            int b1 = qBlue(p1);

            int r2 = qRed(p2);
            int g2 = qGreen(p2);
            int b2 = qBlue(p2);

            // scale img1 channels to [off,255]
            // new = off + round(orig * (255 - off) / 255)
            r1 = off + (r1 * (255 - off)) / 255;
            g1 = off + (g1 * (255 - off)) / 255;
            b1 = off + (b1 * (255 - off)) / 255;

            // scale img2 channels to [0,off]
            // new = round(orig * off / 255)
            r2 = (r2 * off) / 255;
            g2 = (g2 * off) / 255;
            b2 = (b2 * off) / 255;

            line1[x] = qRgb(r1, g1, b1);
            line2[x] = qRgb(r2, g2, b2);
        }
    }

    QImage out(w, h, QImage::Format_RGB32);

    // Merge rule:
    // - For pixels where (x + y) % 2 == 0, take from img1
    // - Otherwise, take from img2
    for (int y = 0; y < h; ++y) {
        QRgb *outLine = reinterpret_cast<QRgb*>(out.scanLine(y));
        const QRgb *line1 = reinterpret_cast<const QRgb*>(img1.scanLine(y));
        const QRgb *line2 = reinterpret_cast<const QRgb*>(img2.scanLine(y));
        for (int x = 0; x < w; ++x) {
            if (((x + y) & 1) == 0) {
                outLine[x] = line1[x];
            } else {
                outLine[x] = line2[x];
            }
        }
    }

    ui->labelOut->setPixmap(QPixmap::fromImage(out));
    m_mergedImage = out;
    on_horizontalSlider_valueChanged(ui->horizontalSlider->value());
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
    if (m_mergedImage.isNull()) {
        return;
    }

    QImage img = m_mergedImage.convertToFormat(QImage::Format_RGB32);
    int w = img.width();
    int h = img.height();

    QImage out(w, h, QImage::Format_RGB32);
    for (int y = 0; y < h; ++y) {
        const QRgb *inLine = reinterpret_cast<const QRgb*>(img.scanLine(y));
        QRgb *outLine = reinterpret_cast<QRgb*>(out.scanLine(y));
        for (int x = 0; x < w; ++x) {
            int r = qRed(inLine[x]);
            int g = qGreen(inLine[x]);
            int b = qBlue(inLine[x]);

            const double minF = 1.0 / 12.0;
            const double maxF = 12.0;
            double factor;
            if (value <= 0) {
                factor = minF;
            } else if (value >= 1023) {
                factor = maxF;
            } else {
                const double t = double(value) / 1024.0;
                factor = minF * std::pow(maxF / minF, t);
            }
            int nr = qBound(0, int(r * factor), 255);
            int ng = qBound(0, int(g * factor), 255);
            int nb = qBound(0, int(b * factor), 255);

            outLine[x] = qRgb(nr, ng, nb);
        }
    }

    ui->labelOut->setPixmap(QPixmap::fromImage(out));
}
