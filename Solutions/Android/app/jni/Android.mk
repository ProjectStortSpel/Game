LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Sources
LOCAL_MODULE    := MySDLMainActivitiy

INPUT_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/Input/ -name "*.cpp" -type f -printf "%P \n")
INPUT_SRC_FILES := $(addprefix ../../../../Source/Input/, $(INPUT_SRC_FILES))

LOCAL_SRC_FILES := main_c.cpp $(INPUT_SRC_FILES)

# Includes
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../Source/

# Libs
LOCAL_SHARED_LIBRARIES := SDL2main_shared SDL2_shared
LOCAL_STATIC_LIBRARIES := lua
LOCAL_LDLIBS := -llog
LOCAL_CPPFLAGS := -std=c++0x

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, ../../../../Externals/android/modules)
$(call import-module, SDL2)
$(call import-module, Lua)

