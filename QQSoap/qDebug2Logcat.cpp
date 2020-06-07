/**
 * Description:
 *
 * Author: Gavin
 */

#if defined(ANDROID)
#include <android/log.h>
#include <QDebug>
#include <QByteArray>
#include "qDebug2Logcat.h"


static const char *g_TAG = NULL;
static void messageOutput(QtMsgType type, const QMessageLogContext &context,
    const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);
#if 0
    int prio = ANDROID_LOG_DEBUG;

    switch (type) {
    case QtDebugMsg:
        prio = ANDROID_LOG_DEBUG;
        break;
    case QtWarningMsg:
        prio = ANDROID_LOG_WARN;
        break;
    case QtCriticalMsg:
        prio = ANDROID_LOG_ERROR;
        break;
    case QtInfoMsg:
        prio = ANDROID_LOG_INFO;
    case QtFatalMsg:
        prio = ANDROID_LOG_FATAL;
        abort();
    }
#endif
    //there is a bug: message can be output only when choose ANDROID_LOG_INFO level.
    __android_log_write(ANDROID_LOG_INFO, g_TAG, msg.toLocal8Bit().data());
}

void installLogcatMessageHandler(const char *TAG)
{
    g_TAG = (TAG == 0 ? "QDebug" : TAG);
    qInstallMessageHandler(messageOutput);
}
#endif
