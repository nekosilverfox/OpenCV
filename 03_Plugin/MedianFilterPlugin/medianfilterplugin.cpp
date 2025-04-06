#include "medianfilterplugin.h"

MedianFilterPlugin::MedianFilterPlugin()
{
}

MedianFilterPlugin::~MedianFilterPlugin()
{

}

QString MedianFilterPlugin::description()
{
    return "This plugin applies median blur filters to any image."
           " This plugin's goal is to make us more familiar with the"
           " concept of plugins in general.";
}

void MedianFilterPlugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    cv::medianBlur(inputImage, outputImage, 10);
}
