#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QResizeEvent>
#include <QGraphicsItem>

#include "customgraphicseffect.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAcceptDrops(true);
    ui->graphicsView->setAcceptDrops(false);  // 禁止视图自身接收拖放
    ui->graphicsView->setScene(&scene);         // 将场景绑定到视图

    // 设置 QGraphicsView 允许用户通过鼠标和键盘与场景中的图元（QGraphicsItem）交互
    ui->graphicsView->setInteractive(true);

    // RubberBandDrag 是一种拖拽模式，允许用户通过按住鼠标左键并拖拽，在视图中绘制一个矩形区域（橡皮筋），释放鼠标后会自动选中该区域内所有符合条件的图元。
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

    // 设置橡皮筋选择的条件为“完全包含图元形状”。
    ui->graphicsView->setRubberBandSelectionMode(Qt::ContainsItemShape);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief MainWindow::dragEnterEvent 用户拖拽文件进入窗口 → 触发 dragEnterEvent 进行验证
 * @param event
 */
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    QStringList acceptedFileTypes{"jpg", "png", "bmp"}; // 允许的文件类型

    /* 从文件管理器拖拽一个文件到应用程序时，
     * 操作系统并不是直接传递文件内容，
     * 而是传递一个文件路径的 URL（例如 file:///C:/Users/YourName/image.jpg）
     *
     * MIME（Multipurpose Internet Mail Extensions）
     * 最初是为了标记电子邮件附件的类型（比如文本、图片、音频），
     * 现在被广泛用于标识数据格式。
     *
     */
    if (event->mimeData()->hasUrls() &&
        event->mimeData()->urls().count() == 1)
    {
        QUrl url = event->mimeData()->urls().at(0);  // 获取第一个URL
        QString localPath = url.toLocalFile();       // 转换成本地文件路径（如 "C:/image.jpg"）
        QFileInfo file(localPath);                   // 用文件路径做后续操作
        if(acceptedFileTypes.contains(file.suffix().toLower()))
        {
            event->acceptProposedAction();
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    qDebug() << scene.items().count();

    QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
    QPixmap pixmap;
    if(pixmap.load(file.absoluteFilePath()))
    {
        // ui->graphicsView->resetTransform();
        // scene.clear();

        // 将鼠标坐标从 MainWindow 原点转换到 以 graphicsView 为原点
        QPoint viewPos = ui->graphicsView->mapFromParent(event->pos());

        // 将鼠标坐标从 graphicsView 原点转换到 以 scene 为原点
        QPointF sceneDropPos = ui->graphicsView->mapToScene(viewPos);
        qDebug() << "viewPos" << viewPos << "\tsceneDropPos" << sceneDropPos;

        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
        item->setFlag(QGraphicsItem::ItemIsSelectable);  // 图元可选中
        item->setAcceptedMouseButtons(Qt::LeftButton);   // 左键选中
        scene.addItem(item);
        item->setPos(sceneDropPos);
        ui->graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
    }
    else
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("The image file cannot be read!"));
    }

// 简单的阈值滤镜
#if 0
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
    item->setGraphicsEffect(new CustomGraphicsEffect(this));
    // item->setGraphicsEffect(new QGraphicsBlurEffect(this));
    scene.addItem(item);
#endif
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
}
