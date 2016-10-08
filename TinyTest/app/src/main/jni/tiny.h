//
// Created by amel on 10/5/16.
//
#include <jni.h>

#ifndef TINYTEST_TINY_H
#define TINYTEST_TINY_H

#include <tiny_dnn/tiny_dnn.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace tiny_dnn;
using namespace tiny_dnn::activation;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_golemdigital_tinytest_MainActivity_TestTinyNet(JNIEnv*, jclass, jstring, jstring);

JNIEXPORT jint JNICALL Java_golemdigital_tinytest_Tiny_TestTinyNet(JNIEnv*, jclass, jstring, jstring);

#ifdef __cplusplus
}
#endif

#endif //TINYTEST_TINY_H
