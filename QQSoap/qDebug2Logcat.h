#ifndef Q_DBUG_2_LOGCAT_H
#define Q_DBUG_2_LOGCAT_H

#ifdef ANDROID
    void installLogcatMessageHandler(const char *TAG);
#else
    #define installLogcatMessageHandler(TAG)
#endif

#endif // DDD_H
