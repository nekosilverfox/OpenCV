#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QObject>

#include "cv_plugin_interface.h"
#include "../copy_make_border_plugin/copy_make_border_plugin.h"

#if 1

#define PLUGINS_DIR                         "/Users/fox/雪狸的文件/Programma/OpenCV/07_Computer_Vision/cvplugins"
#define LANGUAGES_DIR                       "/Users/fox/雪狸的文件/Programma/OpenCV/07_Computer_Vision/languages"
#define THEMES_DIR                          "/Users/fox/雪狸的文件/Programma/OpenCV/07_Computer_Vision/themes"
#define FILE_ON_DISK_DYNAMIC_PROPERTY       "absolute_file_path"

#else
#define PLUGINS_DIR                 ((qApp->applicationDirPath()) + ("/cvplugins/"))
#define LANGUAGES_DIR         ((qApp->applicationDirPath()) + ("/languages/"))
#define THEMES_DIR            ((qApp->applicationDirPath()) + ("/themes/"))
#define FILE_ON_DISK_DYNAMIC_PROPERTY       "absolute_file_path"
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSettings(); // First thing's first, load settings

    populatePluginsMenu();
    populateLanguagesMenu();
    populateThemesMenu();

    ui->graphicsView->setScene(&scene);
    scene.addItem(&originalPixmapItem);
    scene.addItem(&processedPixmapItem);

    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    connect(ui->actionAboutQt, &QAction::triggered, this, [=](){qApp->aboutQt();});
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionOpenImage, &QAction::triggered, this, &MainWindow::importImageByDialog);
    connect(ui->actionSaveImage, &QAction::triggered, this, &MainWindow::exportImageByDialog);
    connect(ui->viewOriginalCheck, &QCheckBox::toggled, this, [=](bool checked){
        originalPixmapItem.setVisible(checked);
        processedPixmapItem.setVisible(!checked);
    });
    connect(ui->action_Camera, &QAction::triggered, this, [](){});

}

MainWindow::~MainWindow()
{
    delete currentPlugin;
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("Packt", "Computer_Vision", this);
    currentThemeFile    = settings.value("currentThemeFile",    "").toString();
    currentLanguageFile = settings.value("currentLanguageFile", "").toString();
    currentPluginFile   = settings.value("currentPluginFile",   "").toString();
}

void MainWindow::saveSettings()
{
    QSettings settings("Packt", "Computer_Vision", this);
    settings.setValue("currentThemeFile",       currentThemeFile);
    settings.setValue("currentLanguageFile",    currentLanguageFile);
    settings.setValue("currentPluginFile",      currentPluginFile);
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    else
    {
        QMainWindow::changeEvent(event);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = QMessageBox::warning(this, tr("Exit"), tr("Are you sure you want to exit?"), QMessageBox::Yes, QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        saveSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

/**
 * @brief MainWindow::populatePluginsMenu 加载插件并添加到列表
 */
void MainWindow::populatePluginsMenu()
{
    qDebug() << "Search plugins in dir" << PLUGINS_DIR;
    QDir pluginsDir(PLUGINS_DIR);

    QFileInfoList pluginFiles = pluginsDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

    for(QFileInfo curFile : pluginFiles)
    {
        if(QLibrary::isLibrary(curFile.absoluteFilePath()))
        {  // 如果是插件
            QPluginLoader pluginLoader(curFile.absoluteFilePath(), this);
            CvPluginInterface *plugin = qobject_cast<CvPluginInterface*>(pluginLoader.instance());
            if(plugin)
            {
                QAction *pluginAction = ui->menu_Plugins->addAction(plugin->title());
                pluginAction->setProperty(FILE_ON_DISK_DYNAMIC_PROPERTY, curFile.absoluteFilePath());  // 为菜单项添加一个自定义的动态属性 FILE_ON_DISK_DYNAMIC_PROPERTY，存储当前插件文件的绝对路径（curFile.absoluteFilePath()）

                connect(pluginAction, &QAction::triggered, this, &MainWindow::onPluginActionTriggered);

                if(currentPluginFile == curFile.absoluteFilePath())
                {  // 用 QSetting 恢复配置的时候 currentPluginFile 被写入了
                    pluginAction->trigger();
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Warning"),
                                     QString(tr("Make sure %1 is a correct plugin for this application<br>"
                                                "and it's not in use by some other application!")).arg(curFile.fileName()));
            }
        }
        else
        {
            QMessageBox::warning(this, tr("Warning"),
                                 QString(tr("Make sure only plugins exist in %1 folder.<br>"
                                            "%2 is not a plugin."))
                                     .arg(PLUGINS_DIR)
                                     .arg(curFile.fileName()));
        }
    }

    if(ui->menu_Plugins->actions().count() <= 0)
    {
        QMessageBox::critical(this, tr("No Plugins"), QString(tr("This application cannot work without plugins!"
                                                                 "<br>Make sure that %1 folder exists "
                                                                 "in the same folder as the application<br>and that "
                                                                 "there are some filter plugins inside it")).arg(PLUGINS_DIR));
        this->setEnabled(false);
    }
}

/**
 * @brief MainWindow::populateLanguagesMenu 加载语言并添加到列表
 */
void MainWindow::populateLanguagesMenu()
{
    qDebug() << "Search Languages in dir" << LANGUAGES_DIR;
    QMenu *languagesMenu = new QMenu(this);

    // Add default (english) language
    QAction *defaultLanguageAction = languagesMenu->addAction("English - US");
    defaultLanguageAction->setProperty(FILE_ON_DISK_DYNAMIC_PROPERTY, "");
    connect(defaultLanguageAction, &QAction::triggered, this, &MainWindow::onLanguageActionTriggered);

    // Load all languages and populate the menus
    QDir languagesDir(LANGUAGES_DIR);
    QFileInfoList languageFiles = languagesDir.entryInfoList(QStringList() << "*.qm",
                                                             QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
    foreach(QFileInfo languageFile, languageFiles)
    {
        QAction *languageAction = languagesMenu->addAction(languageFile.baseName());
        languageAction->setProperty(FILE_ON_DISK_DYNAMIC_PROPERTY, languageFile.absoluteFilePath());
        connect(languageAction, &QAction::triggered, this, &MainWindow::onLanguageActionTriggered);

        if(currentLanguageFile == languageFile.absoluteFilePath())
        {
            languageAction->trigger();
        }
    }
    ui->actionLanguage->setMenu(languagesMenu);
}

/**
 * @brief MainWindow::populateThemesMenu 加载主题并添加到列表
 */
void MainWindow::populateThemesMenu()
{
    qDebug() << "Search Themes in dir" << THEMES_DIR;

    QMenu *themesMenu = new QMenu(this);
    // Add default (native) theme
    QAction *defaultThemeAction = themesMenu->addAction("Default");
    defaultThemeAction->setProperty(FILE_ON_DISK_DYNAMIC_PROPERTY, "");
    connect(defaultThemeAction, &QAction::triggered, this, &MainWindow::onThemeActionTriggered);

    // Load all themes and populate the menus
    QDir themesDir(THEMES_DIR);
    QFileInfoList themeFiles = themesDir.entryInfoList(QStringList() << "*.thm", QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
    foreach(QFileInfo themeFile, themeFiles)
    {
        QAction *themeAction = themesMenu->addAction(themeFile.baseName());
        themeAction->setProperty(FILE_ON_DISK_DYNAMIC_PROPERTY, themeFile.absoluteFilePath());
        connect(themeAction, &QAction::triggered, this, &MainWindow::onThemeActionTriggered);

        if(currentThemeFile == themeFile.absoluteFilePath())
        {
            themeAction->trigger();
        }
    }
    ui->actionTheme->setMenu(themesMenu);
}


void MainWindow::onPluginActionTriggered(bool)
{
    if(!currentPlugin.isNull())
    {
        delete currentPlugin;
        delete currentPluginGui;
    }

    currentPluginFile = QObject::sender()->property(FILE_ON_DISK_DYNAMIC_PROPERTY).toString();
    currentPlugin = new QPluginLoader(currentPluginFile, this);
    qDebug() << "Try to load plugin" << currentPluginFile;

    // 关键修改点：使用 qobject_cast 替代 dynamic_cast
    CvPluginInterface* currentPluginInstance = qobject_cast<CvPluginInterface*>(currentPlugin->instance());
    if(currentPluginInstance)
    {
        currentPluginGui = new QWidget(this);
        ui->pluginLayout->addWidget(currentPluginGui);
        currentPluginInstance->setupUi(currentPluginGui);  // currentPluginGui 是 parent
        connect(currentPluginInstance, &CvPluginInterface::updateNeeded, this, &MainWindow::onCurrentPluginUpdateNeeded);
        connect(currentPluginInstance, &CvPluginInterface::infoMessage,  this, &MainWindow::onCurrentPluginInfoMessage);
        connect(currentPluginInstance, &CvPluginInterface::errorMessage, this, &MainWindow::onCurrentPluginErrorMessage);
    }
}

void MainWindow::onLanguageActionTriggered(bool)
{
    currentLanguageFile = QObject::sender()->property(FILE_ON_DISK_DYNAMIC_PROPERTY).toString();
    if(!currentLanguageFile.isEmpty())
    {
        qApp->removeTranslator(&translator);
        translator.load(currentLanguageFile);
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
    }
}

void MainWindow::onThemeActionTriggered(bool)
{
    currentThemeFile = QObject::sender()->property(FILE_ON_DISK_DYNAMIC_PROPERTY).toString();
    QFile themeFile(currentThemeFile);
    if(currentThemeFile.isEmpty())
    {
        qApp->setStyleSheet("");
    }
    else
    {
        themeFile.open(QFile::ReadOnly | QFile::Text);
        QString styleSheet = themeFile.readAll();
        qApp->setStyleSheet(styleSheet);
        themeFile.close();
    }
}

/**
 * @brief MainWindow::importImageByDialog 使用文件对话框来导入图像
 */
void MainWindow::importImageByDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Input Image"),
                                                    QDir::currentPath(),
                                                    tr("Images") + " (*.jpg *.png *.bmp)");

    originalMat = cv::imread(fileName.toStdString());
    if(!originalMat.empty())
    {
        onCurrentPluginUpdateNeeded();
    }
    else if(!fileName.trimmed().isEmpty())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Make sure the image file exists "
                                 "and it is accessible!"));
    }
}

void MainWindow::exportImageByDialog()
{
    if(!ui->viewOriginalCheck->isChecked() && !processedMat.empty())
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), QDir::currentPath(), "*.jpg;;*.png;;*.bmp");
        if(!fileName.isEmpty())
        {
            cv::imwrite(fileName.toStdString(), processedMat);
        }
    }
    else if(ui->viewOriginalCheck->isChecked() && !originalMat.empty())
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), QDir::currentPath(), "*.jpg;;*.png;;*.bmp");
        if(!fileName.isEmpty())
        {
            cv::imwrite(fileName.toStdString(), originalMat);
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("There is nothing to be saved!"));
    }
}

void MainWindow::onCurrentPluginUpdateNeeded()
{
    qDebug() << "Update image";

    if(!originalMat.empty())
    {
        if(!currentPlugin.isNull())
        {
            CvPluginInterface *currentPluginInstance = qobject_cast<CvPluginInterface*>(currentPlugin->instance());
            if(currentPluginInstance)
            {
                cv::TickMeter meter;
                meter.start();
                currentPluginInstance->processImage(originalMat, processedMat);
                meter.stop();
                qDebug() << "The process took " << meter.getTimeMilli() << "milliseconds";
            }
        }
        else
        {
            processedMat = originalMat.clone();
        }

        originalImage = QImage(originalMat.data, originalMat.cols, originalMat.rows, originalMat.step, QImage::Format_RGB888);
        originalPixmapItem.setPixmap(QPixmap::fromImage(originalImage.rgbSwapped()));

        processedImage = QImage(processedMat.data, processedMat.cols, processedMat.rows, processedMat.step, QImage::Format_RGB888);
        processedPixmapItem.setPixmap(QPixmap::fromImage(processedImage.rgbSwapped()));
    }
}

void MainWindow::onCurrentPluginErrorMessage(const QString &msg)
{
    qDebug() << "Plugin Error Message : " << msg;
}

void MainWindow::onCurrentPluginInfoMessage(const QString &msg)
{
    qDebug() << "Plugin Info Message : " << msg;
}
