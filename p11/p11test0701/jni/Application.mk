APP_MODULES := p11test
APP_PLATFORM:=android-19
APP_CPPFLAGS += -fexceptions

LOCAL_CERTIFICATE := platform

#APP_CPPFLAGS := -std=c++11
LOCAL_CFLAGS := -std=c++11
APP_STL := stlport_static

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

APP_CPPFLAGS += -frtti
APP_ABI:=armeabi-v7a 
#APP_ABI:=arm64-v8a 
#arm64-v8a