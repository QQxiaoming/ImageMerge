#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imagemerge.h"
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

    QImage out;
    ImageMerge::mergeImages(pix1.toImage(), pix2.toImage(), offset(), out);

    ui->labelOut->setPixmap(QPixmap::fromImage(out));
    m_mergedImage = out;
    on_horizontalSlider_valueChanged(ui->horizontalSlider->value());
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
    if (m_mergedImage.isNull()) {
        return;
    }
    QImage out;
    ImageMerge::exposeImage(m_mergedImage, value, out);
    ui->labelOut->setPixmap(QPixmap::fromImage(out));
}
