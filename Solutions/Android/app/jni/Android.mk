# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Important: The name of this module cannot change because it is hardcoded in the Android libSDLmain.so
LOCAL_MODULE    := MySDLMainActivitiy
#LOCAL_SRC_FILES := main_c.c SDL_android_main.c
LOCAL_SRC_FILES := main_c.c 

#LOCAL_SHARED_LIBRARIES := ALmixer_shared openal_shared SDL2_shared SDL2main_shared
LOCAL_SHARED_LIBRARIES := SDL2main_shared SDL2_shared
#LOCAL_LDLIBS := -llog
LOCAL_LDLIBS :=
LOCAL_STATIC_LIBRARIES := lua



include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, ../../../../Externals/android/modules)
# Remember: The NDK_MODULE_PATH environmental variable must contain the modules directories in the search path.
# android build system will look for folder `ALmixer` and `openal` and 'SDL2'
# in all import paths:
#$(call import-module,ALmixer)
#$(call import-module,openal-soft)
$(call import-module,SDL2)
$(call import-module,Lua)

