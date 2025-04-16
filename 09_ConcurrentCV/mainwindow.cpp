#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnProcessInLoop, &QPushButton::clicked, this, &MainWindow::processInLoop);
    connect(ui->btnProcessConcurrently, &QPushButton::clicked, this, &MainWindow::processInConcurrent);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QFileInfoList MainWindow::getImagesInFolder()
{
    QDir dir(QFileDialog::getExistingDirectory(this, tr("Open Images Folder")));

    return dir.entryInfoList(QStringList() << "*.jpg" << "*.png",
                             QDir::NoDotAndDotDot | QDir::Files,
                             QDir::Name);
}

/**
 * @brief MainWindow::addDateTimeOnImage 再图像左上角添加一个时间
 * @param info
 */
void MainWindow::addDateTimeOnImage(QFileInfo &info)
{
    cv::Mat image = cv::imread(info.absoluteFilePath().toStdString());
    if (image.empty())
    {
        return;
    }

    QString date = info.birthTime().toString();
    cv::putText(image,
                date.toStdString(),
                cv::Point(80, 80), // 25 pixels offset from the corner
                cv::FONT_HERSHEY_PLAIN,
                1.0,
                cv::Scalar(0, 0, 255));  // red

    cv::imwrite(info.absoluteFilePath().toStdString(), image);
}

/**
 * @brief MainWindow::filterImage 返回布尔值的过滤函数来筛选早于2025年的图像
 * @param info
 * @return
 */
bool MainWindow::filterImage(QFileInfo &info)
{
    return info.birthTime().date().year() < 2025;
}

void MainWindow::processInLoop()
{
    QFileInfoList fileList = getImagesInFolder();

    QElapsedTimer elapsedTimer;
    elapsedTimer.start();

    ui->barProcess->setRange(0, fileList.count() - 1);
    for (int i = 0; i < fileList.count(); i++)
    {
        addDateTimeOnImage(fileList[i]);
        ui->barProcess->setValue(i);
        qApp->processEvents();
    }

    qint64 e = elapsedTimer.elapsed();
    QMessageBox::information(this,
                             tr("Done!"),
                             QString(tr("Processed %1 images in %2 milliseconds"))
                                 .arg(fileList.count())
                                 .arg(e));
}

void MainWindow::processInConcurrent()
{
    QFileInfoList fileList = getImagesInFolder();

    QElapsedTimer elapsedTimer;
    elapsedTimer.start();

    /*
     *  map 自动利用多核并行处理，将特定函数应用于列表中的所有元素
     *  第二个参数必须是 static 的
     */
    QFuture<void> future = QtConcurrent::map(fileList, addDateTimeOnImage);
    QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);

    // 当任务的进度范围, 通常用于初始化或重置进度条的显示范围（如 0 到 100）
    connect(watcher, &QFutureWatcher<void>::progressRangeChanged,
            ui->barProcess, &QProgressBar::setRange);

    // 当任务的当前进度值发生变化时触发。用于更新进度条的当前值
    connect(watcher, &QFutureWatcher<void>::progressValueChanged,
            ui->barProcess, &QProgressBar::setValue);

    connect(watcher, &QFutureWatcher<void>::finished,
            this, [=](){
        qint64 e = elapsedTimer.elapsed();
        QMessageBox::information(this, tr("Done!"),
                                 QString(tr("Processed %1 images in %2 milliseconds"))
                                     .arg(fileList.count())
                                     .arg(e));
    });

    connect(watcher, &QFutureWatcher<void>::finished,
            watcher, &QFutureWatcher<void>::deleteLater);

    watcher->setFuture(future);
}



