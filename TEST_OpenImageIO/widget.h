#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void convertToGrayscale(const std::string &inputPath, const std::string &outputPath);
    void resizeImage(const std::string &inputPath, const std::string &outputPath, int newWidth, int newHeight);
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
