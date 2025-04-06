#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QPluginLoader>
#include <QFileInfoList>
#include "opencv2/opencv.hpp"
#include "../CvPluginInterface/cvplugininterface.h"

#define FILTERS_SUBFOLDER "/filter_plugins/"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 加载插件
    getPluginsList();

    connect(ui->btnSelectImage, &QPushButton::clicked, this, &MainWindow::selectImage);
    connect(ui->btnHelp, &QPushButton::clicked, this, &MainWindow::showPluginDescription);
    connect(ui->btnFilter, &QPushButton::clicked, this, &MainWindow::useFilterOnImage);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectImage()
{
    QString fileName =
        QFileDialog::getOpenFileName(
            this,
            tr("Open Input Image"),
            QDir::currentPath(),
            tr("Images") + " (*.jpg *.png *.bmp)");
    if(QFile::exists(fileName))
    {
        ui->leInputImage->setText(fileName);
    }
}

/**
 * @brief MainWindow::showPluginDescription 点击帮助按钮将显示插件中 `description` 函数的结果
 */
void MainWindow::showPluginDescription()
{
   if(ui->lwFilters->currentRow() < 0)
  {
    QMessageBox::warning(this, tr("Warning"), QString(tr("First select a filter plugin from the list.")));
       return;
  }

  QString pluginPath = qApp->applicationDirPath() + FILTERS_SUBFOLDER + ui->lwFilters->currentItem()->text();
  qDebug() << "Plugin path" << pluginPath;

  QPluginLoader pluginLoader(pluginPath);
  CvPluginInterface *plugin = dynamic_cast<CvPluginInterface*>(pluginLoader.instance());

  if(plugin)
  {
      QMessageBox::information(this, tr("Plugin Description"),
                               plugin->description());
  }
  else
  {
      QMessageBox::warning(this, tr("Warning"),
                           QString(tr("Make sure plugin %1 exists and is usable.")) .arg(ui->lwFilters->currentItem()->text()));
  }
}

/**
 * @brief MainWindow::useFilterOnImage 将选择的滤镜应用于图片
 */
void MainWindow::useFilterOnImage()
{
    if(ui->lwFilters->currentRow() < 0 || ui->leInputImage->text().isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             QString(tr( "First select a filter plugin from the list." )));
        return;
    }

    QPluginLoader pluginLoader(qApp->applicationDirPath() + FILTERS_SUBFOLDER + ui->lwFilters->currentItem()->text());
    CvPluginInterface *plugin = dynamic_cast<CvPluginInterface*>(pluginLoader.instance());
    if(nullptr == plugin)
    {
        QMessageBox::warning(this, tr("Warning"),
                             QString(tr("Make sure plugin %1 exists and is usable.")).arg(ui->lwFilters->currentItem()->text()));
    }

    if(QFile::exists(ui->leInputImage->text()))
    {
        using namespace cv;
        Mat inputImage, outputImage;
        inputImage = imread(ui->leInputImage-> text().toStdString());
        plugin->processImage(inputImage, outputImage);
        imshow(tr("Filtered Image").toStdString(), outputImage);
    }
    else
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             QString(tr("Make sure %1 exists.")).arg(ui->leInputImage->text()));
    }
}


/**
 * @brief MainWindow::getPluginsList 获取插件列表
 */
void MainWindow::getPluginsList()
{
    QString findDir = qApp->applicationDirPath() + FILTERS_SUBFOLDER;
    qDebug() << "Search plugin in dir:" << findDir;

    QDir filtersDir(findDir);
    QFileInfoList filters = filtersDir.entryInfoList( QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
    foreach(QFileInfo filter, filters)
    {
        if(QLibrary::isLibrary(filter.absoluteFilePath()))  // 确保只接受插件（库）文件
        {
            QPluginLoader pluginLoader(filter.absoluteFilePath(), this);
            if(dynamic_cast<CvPluginInterface*>(pluginLoader.instance()))
            {
                ui->lwFilters->addItem(filter.fileName());
                pluginLoader.unload(); // we can unload for now
            }
            else
            {
                QMessageBox::warning(
                    this, tr("Warning"),
                    QString(tr("Make sure %1 is a correct"
                               " plugin for this application<br>"
                               "and it's not in use by some other"
                               " application!"))
                        .arg(filter.fileName()));
            }
        }
        else
        {
            QMessageBox::warning(this, tr("Warning"),
                                 QString(tr("Make sure only plugins"
                                            " exist in plugins folder.<br>"
                                            "%1 is not a plugin."))
                                     .arg(filter.fileName()));
        }
    }

    if(ui->lwFilters->count() <= 0)
    {
        QMessageBox::critical(this, tr("No Plugins"),
                              tr("This application cannot work without plugins!"
                                 "<br>Make sure that filter_plugins folder exists "
                                 "in the same folder as the application<br>and that "
                                 "there are some filter plugins inside it"));
        this->setEnabled(false);  // 仅禁用了当前窗口所有控件
    }
}
