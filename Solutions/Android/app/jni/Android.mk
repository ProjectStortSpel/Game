LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Sources
LOCAL_MODULE    := MySDLMainActivitiy

LOCAL_CPPFLAGS := -std=c++0x

ifeq ($(HOST_OS), windows)

AUDIO_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\Audio\*.cpp') do @echo %%~dpnxA)
FILESYSTEM_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\FileSystem\*.cpp') do @echo %%~dpnxA)
LOGGER_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\Logger\*.cpp') do @echo %%~dpnxA)
MPL_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\MPL\*.cpp') do @echo %%~dpnxA)
INPUT_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\Input\*.cpp') do @echo %%~dpnxA)
CONSOLE_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\Console\*.cpp') do @echo %%~dpnxA)
LUAEMBEDDER_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\LuaEmbedder\*.cpp') do @echo %%~dpnxA)
NETWORK_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\Network\*.cpp') do @echo %%~dpnxA)
ECSL_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\ECSL\*.cpp') do @echo %%~dpnxA)
RENDERER_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\AndroidRenderer\*.cpp') do @echo %%~dpnxA)
GAME_SRC_FILES := $(shell for /F "tokens=*" %%A in ('dir /s /b %CD%\..\..\..\..\Source\Game\*.cpp') do @echo %%~dpnxA)

else ifeq ($(HOST_OS), linux)

AUDIO_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/Audio/ -name "*.cpp" -type f -printf "%P \n")
AUDIO_SRC_FILES := $(addprefix ../../../../Source/Audio/, $(AUDIO_SRC_FILES))
FILESYSTEM_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/FileSystem/ -name "*.cpp" -type f -printf "%P \n")
FILESYSTEM_SRC_FILES := $(addprefix ../../../../Source/FileSystem/, $(FILESYSTEM_SRC_FILES))
LOGGER_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/Logger/ -name "*.cpp" -type f -printf "%P \n")
LOGGER_SRC_FILES := $(addprefix ../../../../Source/Logger/, $(LOGGER_SRC_FILES))
MPL_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/MPL/ -name "*.cpp" -type f -printf "%P \n")
MPL_SRC_FILES := $(addprefix ../../../../Source/MPL/, $(MPL_SRC_FILES))
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
RENDERER_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/AndroidRenderer/ -name "*.cpp" -type f -printf "%P \n")
RENDERER_SRC_FILES := $(addprefix ../../../../Source/AndroidRenderer/, $(RENDERER_SRC_FILES))
GAME_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../../Source/Game/ -name "*.cpp" -type f -printf "%P \n")
GAME_SRC_FILES := $(addprefix ../../../../Source/Game/, $(GAME_SRC_FILES))

endif

LOCAL_SRC_FILES := $(AUDIO_SRC_FILES) $(FILESYSTEM_SRC_FILES) $(LOGGER_SRC_FILES) $(MPL_SRC_FILES) $(INPUT_SRC_FILES) $(CONSOLE_SRC_FILES) $(LUAEMBEDDER_SRC_FILES) $(NETWORK_SRC_FILES) $(ECSL_SRC_FILES) $(RENDERER_SRC_FILES) $(GAME_SRC_FILES)

# Includes
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../Source/ $(LOCAL_PATH)/../../../../Externals/android/include/

# Libs
LOCAL_SHARED_LIBRARIES := SDL2main_shared SDL2_shared SDL2_ttf_shared mikmod_shared smpeg2_shared SDL2_mixer_shared libandroid
LOCAL_STATIC_LIBRARIES := lua
LOCAL_LDLIBS := -llog -landroid -lGLESv2

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, ../../../../Externals/android/modules)
$(call import-module, SDL2)
$(call import-module, Lua)

