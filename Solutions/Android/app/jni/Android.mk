LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Sources
LOCAL_MODULE    := MySDLMainActivitiy

LOCAL_CPPFLAGS := -std=c++0x

INPUT_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/Input/ -name "*.cpp" -type f -printf "%P \n")
INPUT_SRC_FILES := $(addprefix ../../../../Source/Input/, $(INPUT_SRC_FILES))

CONSOLE_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/Console/ -name "*.cpp" -type f -printf "%P \n")
CONSOLE_SRC_FILES := $(addprefix ../../../../Source/Console/, $(CONSOLE_SRC_FILES))

LUAEMBEDDER_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/LuaEmbedder/ -name "*.cpp" -type f -printf "%P \n")
LUAEMBEDDER_SRC_FILES := $(addprefix ../../../../Source/LuaEmbedder/, $(LUAEMBEDDER_SRC_FILES))

NETWORK_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/Network/ -name "*.cpp" -type f -printf "%P \n")
NETWORK_SRC_FILES := $(addprefix ../../../../Source/Network/, $(NETWORK_SRC_FILES))

ECSL_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/ECSL/ -name "*.cpp" -type f -printf "%P \n")
ECSL_SRC_FILES := $(addprefix ../../../../Source/ECSL/, $(ECSL_SRC_FILES))

LOCAL_SRC_FILES := main_c.cpp $(INPUT_SRC_FILES) $(CONSOLE_SRC_FILES) $(LUAEMBEDDER_SRC_FILES) $(NETWORK_SRC_FILES) $(ECSL_SRC_FILES)

# Includes
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../Source/ $(LOCAL_PATH)/../../../../Externals/android/include/

# Libs
LOCAL_SHARED_LIBRARIES := SDL2main_shared SDL2_shared
LOCAL_STATIC_LIBRARIES := lua
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, ../../../../Externals/android/modules)
$(call import-module, SDL2)
$(call import-module, Lua)

