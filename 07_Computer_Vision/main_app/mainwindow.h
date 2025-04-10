#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>
#include <QFileInfoList>
#include <QLibrary>
#include <QPluginLoader>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QTranslator>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>
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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

private slots:
    void onPluginActionTriggered(bool);
    void onLanguageActionTriggered(bool);
    void onThemeActionTriggered(bool);
    void onCurrentPluginUpdateNeeded();
    void onCurrentPluginErrorMessage(const QString &msg);
    void onCurrentPluginInfoMessage(const QString &msg);

    void importImageByDialog();
    void exportImageByDialog();

private:
    void loadSettings();
    void saveSettings();
    void populatePluginsMenu();
    void populateLanguagesMenu();
    void populateThemesMenu();

private:
    Ui::MainWindow *ui;

    QString currentThemeFile;
    QString currentLanguageFile;
    QString currentPluginFile;

    QPointer<QPluginLoader> currentPlugin;
    QPointer<QWidget> currentPluginGui;
    QGraphicsScene scene;
    QTranslator translator;
    QGraphicsPixmapItem originalPixmap, processedPixmap;
    cv::Mat originalMat, processedMat;
    QImage originalImage, processedImage;
};
#endif // MAINWINDOW_H
