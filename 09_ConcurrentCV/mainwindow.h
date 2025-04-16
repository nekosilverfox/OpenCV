#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QFileInfoList>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QFileInfoList getImagesInFolder();
    static void addDateTimeOnImage(QFileInfo &info);
    static bool filterImage(QFileInfo &info);

public slots:
    void processInLoop();
    void processInConcurrent();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
