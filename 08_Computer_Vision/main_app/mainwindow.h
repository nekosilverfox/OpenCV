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

    /*
     * QPointer 是 Qt 框架中专门为管理 QObject 及其派生类对象设计的智能指针工具，其核心功能是​​自动检测对象生命周期​​，避免指针悬空问题
     * 但是 QPointer 不能用于非 QObject 类型的对象
     * QPointer 仅跟踪对象是否存在，不会自动释放内存。仍需手动调用 delete 或结合其他智能指针（如 QSharedPointer）管理所有权
     * 在多线程环境中，若对象被其他线程销毁，需通过同步机制（如互斥锁）确保 QPointer 的访问安全
     */
    QPointer<QPluginLoader> currentPlugin;
    QPointer<QWidget> currentPluginGui;
    QGraphicsScene scene;
    QTranslator translator;
    QGraphicsPixmapItem originalPixmapItem, processedPixmapItem;
    cv::Mat originalMat, processedMat;
    QImage originalImage, processedImage;
};
#endif // MAINWINDOW_H
