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
                cv::Point(240, 240), // 25 pixels offset from the corner
                cv::FONT_HERSHEY_PLAIN,
                1.0,
                cv::Scalar(0, 0, 255));  // red

    cv::imwrite(info.absoluteFilePath().toStdString(), image);
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

    connect(watcher, &QFutureWatcher<void>::progressRangeChanged,
            ui->barProcess, &QProgressBar::setRange);

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



