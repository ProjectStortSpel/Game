# Despite what you may have heard, it is best to set this to the highest value possible.
# Backwards compatibiltiy is ensured by merely not calling APIs that don't exist on older systems.
# (SDL already does this for you if you are using SDL's APIs.)
# (Android fixes and tests bugs in the latest, not the oldest, and in my experience, 
# relying on foward compatibility reveals strange runtime error bugs on newer OS's and devices
# that are really hard to diagnose and nobody at Google tests for because they all test the latest.)
APP_PLATFORM := android-19
APP_ABI := armeabi armeabi-v7a x86
APP_OPTIM := release
#APP_OPTIM := debug
APP_STL := stlport_static
STLPORT_FORCE_REBUILD := true