#include "gaussian_filter_plugin.h"

GaussianFilterPlugin::GaussianFilterPlugin() {}

GaussianFilterPlugin::~GaussianFilterPlugin()
{

}

QString GaussianFilterPlugin::description()
{
    return "This is a plugin for Gaussian";
}

void GaussianFilterPlugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
     cv::medianBlur(inputImage, outputImage, 99);
}



