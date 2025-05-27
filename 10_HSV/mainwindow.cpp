#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    using namespace cv;


    Mat image(25, 180, CV_8UC3);
    for(int i=0; i<image.rows; i++)
    {
        for(int j=0; j<image.cols; j++)
        {
            image.at<Vec3b>(i,j)[0] = j;
            image.at<Vec3b>(i,j)[1] = 255;
            image.at<Vec3b>(i,j)[2] = 255;
        }
    }
    cvtColor(image,image,COLOR_HSV2BGR);
    imshow("Hue", image);


    // 读取图像
    Mat img = imread("/Users/fox/Desktop/IMG_0365_副本.JPG");

    // 转换为HSV颜色空间
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    // 定义颜色范围
    // 红色（两个区间）
    Scalar lower_red1(0, 50, 50);
    Scalar upper_red1(10, 255, 255);
    Scalar lower_red2(170, 50, 50);
    Scalar upper_red2(180, 255, 255);

    // 洋红色
    Scalar lower_magenta(150, 50, 50);
    Scalar upper_magenta(170, 255, 255);

    // 创建掩膜
    Mat mask_red1, mask_red2, mask_red, mask_magenta, combined_mask;
    inRange(hsv, lower_red1, upper_red1, mask_red1);
    inRange(hsv, lower_red2, upper_red2, mask_red2);
    bitwise_or(mask_red1, mask_red2, mask_red);
    inRange(hsv, lower_magenta, upper_magenta, mask_magenta);
    bitwise_or(mask_red, mask_magenta, combined_mask);  // 合并掩膜

    // 创建带颜色的掩膜显示
    Mat colored_mask = Mat::zeros(img.size(), img.type());
    colored_mask.setTo(Scalar(0, 0, 255), mask_red);     // 红色区域
    colored_mask.setTo(Scalar(255, 0, 255), mask_magenta); // 洋红色区域

    // 计算直方图参数
    int channels[] = {0};
    int histSize[] = {180};
    float range[] = {0, 180};
    const float* histRange[] = {range};

    // 计算直方图
    Mat red_hist, magenta_hist;
    calcHist(&hsv, 1, channels, mask_red, red_hist, 1, histSize, histRange);
    calcHist(&hsv, 1, channels, mask_magenta, magenta_hist, 1, histSize, histRange);

    // 归一化
    normalize(red_hist, red_hist, 0, 255, NORM_MINMAX);
    normalize(magenta_hist, magenta_hist, 0, 255, NORM_MINMAX);

    // 创建叠加直方图
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize[0]);
    Mat combinedHistImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    // 绘制双直方图
    for (int i = 0; i < histSize[0]; i++) {
        // 红色直方图
        rectangle(combinedHistImage,
                  Point(bin_w * i, hist_h),
                  Point(bin_w * (i + 1), hist_h - cvRound(red_hist.at<float>(i))),
                  Scalar(0, 0, 255),  // BGR格式：红色
                  FILLED);

        // 洋红色直方图（半透明叠加）
        rectangle(combinedHistImage,
                  Point(bin_w * i, hist_h),
                  Point(bin_w * (i + 1), hist_h - cvRound(magenta_hist.at<float>(i))),
                  Scalar(255, 0, 255),  // BGR格式：洋红色
                  FILLED);
    }

    // 显示结果
    // imshow("原始图像", img);
    imshow("合并掩膜", combined_mask);
    // imshow("彩色掩膜叠加", colored_mask);
    // imshow("组合直方图", combinedHistImage);

    // 可选：显示掩膜应用效果
    Mat result;
    bitwise_and(img, img, result, combined_mask);
    // imshow("掩膜应用效果", result);
}

MainWindow::~MainWindow()
{
    delete ui;
}
