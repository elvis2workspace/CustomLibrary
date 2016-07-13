LOCAL_PATH := $(call my-dir)

LOCAL_CERTIFICATE := platform

#include $(CLEAR_VARS)
#LOCAL_MODULE    := p11testxx
#LOCAL_SRC_FILES :=D:/Android/android-ndk-r10e/sources/cxx-stl/stlport/libs/armeabi/libstlport_static.a
#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= p11test

LOCAL_LDLIBS := -llog  -lstdc++


#LOCAL_STATIC_LIBRARIES := p11testxx -log
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/p11
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/test
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/sm234
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/tinyxml
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/minixml

#LOCAL_C_INCLUDES += D:/Android/android-ndk-r10e/sources/cxx-stl/stlport/stlport

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/p11test/*.c)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/p11test/*.cpp)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/tinyxml/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/sm234/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/minixml/*.c)
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)
include $(BUILD_EXECUTABLE)

