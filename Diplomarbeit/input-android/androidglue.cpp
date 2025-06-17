#include "androidglue.h"
#include <jni.h>



static int usbDescriptor = -1;
static int usbVendorid = -1;
static int usbProductid = -1;
static int usbEndPoint = -1;
static bool isDevConnected = false;

extern "C" {
    JNIEXPORT void JNICALL Java_at_beatthescore_JNIUtils_setConnected(JNIEnv * env, jobject obj,
                    jint vendorId, jint productId, jint endpoint, jint descriptor);
    JNIEXPORT void JNICALL Java_at_beatthescore_JNIUtils_setConnected(JNIEnv * env, jobject obj,
                    jint vendorId, jint productId, jint endpoint, jint descriptor)
    {
        usbVendorid = (int)vendorId;
        usbProductid = (int)productId;
        usbDescriptor = (int)descriptor;
        usbEndPoint = (int)endpoint;
        if(usbDescriptor == -1) {
            isDevConnected = false;
        } else {
            isDevConnected = true;
        }
    }
}

AndroidGlue::AndroidGlue() : QObject(0)
{
    isDevConnected = false;
}

bool AndroidGlue::isConnected()
{
    return isDevConnected;
}

int AndroidGlue::getDescriptor()
{
    return usbDescriptor;
}

int AndroidGlue::getProductID()
{
    return usbProductid;
}

int AndroidGlue::getVendorID()
{
    return usbVendorid;
}

int AndroidGlue::getEndpoint()
{
    return usbEndPoint;
}

void AndroidGlue::reset()
{
    usbDescriptor = -1;
    usbProductid = -1;
    usbVendorid = -1;
    usbEndPoint = -1;
    isDevConnected = false;
}
