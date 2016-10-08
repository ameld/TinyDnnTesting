//
// Created by amel on 10/5/16.
//

#include "tiny.h"

// convert image to vec_t
void convert_image(const std::string& imagefilename, double scale, int w, int h, vec_t &data)
{
    cv::Mat img = cv::imread(imagefilename, cv::IMREAD_GRAYSCALE);
    if (img.data == nullptr) return; // cannot open, or it's not an image

    cv::Mat_<uint8_t> resized;
    cv::resize(img, resized, cv::Size(w, h));

    //cv::normalize(resized, resized, 0, 1, cv::NORM_MINMAX);

    vec_t d;

    std::transform(resized.begin(), resized.end(), std::back_inserter(data),
                   [=](uint8_t c) { return c * scale; });
}


int predictTiny(std::string net_path, std::string img_path)
{
    network<sequential> net;
    //construct_net(net);
    net.load(net_path);

    vec_t img;
    convert_image(img_path, 1, 32, 32, img);

    return int(net.predict_label(img));
}

//--------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_golemdigital_tinytest_MainActivity_TestTinyNet(JNIEnv *j_env,
  jclass j_class, jstring net_file, jstring image_file)
{
    const std::string net = (*j_env).GetStringUTFChars(net_file, JNI_FALSE);
    const std::string img = (*j_env).GetStringUTFChars(image_file, JNI_FALSE);

    return jint(predictTiny(net, img));

}



//--------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_golemdigital_tinytest_Tiny_TestTinyNet(JNIEnv *j_env,
   jclass j_class, jstring net_file, jstring image_file)
{
    const std::string net = (*j_env).GetStringUTFChars(net_file, JNI_FALSE);
    const std::string img = (*j_env).GetStringUTFChars(image_file, JNI_FALSE);

    return jint(predictTiny(net, img));
}

