#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // set the offset used when remapping channel ranges
    void setOffset(int offset);
    int offset() const { return m_offset; }

private slots:
    void on_pushButtonOpen_clicked();
    void on_pushButtonOpen2_clicked();
    void on_pushButtonMerge_clicked();
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    int m_offset = 30;
    QImage m_mergedImage;
};

#endif // MAINWINDOW_H
