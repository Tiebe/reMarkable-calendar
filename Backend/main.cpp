#include <QObject>
#include <QProcess>
#include <QString>
#include <QQmlApplicationEngine>
#include <dlfcn.h>
#include "Calendar.h"
#include "datefilterproxymodel.h"


extern "C" void _xovi_construct(){
    qmlRegisterType<Calendar>("dev.tiebe.Calendar", 1, 0, "Calendar");
    qmlRegisterType<DateFilterProxyModel>("dev.tiebe.Calendar", 1, 0, "DateFilterProxyModel");
}

extern "C" char _xovi_shouldLoad() {
    // Only attach self to GUI applications
    void *resFunc = dlsym(RTLD_DEFAULT, "_Z21qRegisterResourceDataiPKhS0_S0_");
    if(resFunc == NULL) {
        printf("[Calendar]: Not a GUI application. Refusing to load.\n");
        return 0;
    }
    return 1;
}

extern "C" __attribute__((section(".xovi_info"))) const int EXTENSIONVERSION = 0x00000100;
