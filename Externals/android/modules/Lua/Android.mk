LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := lua
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/liblua.a
LOCAL_EXPORT_CFLAGS := -DLUA_REAL_FLOAT
LOCAL_EXPORT_LDLIBS := -fPIC
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)
