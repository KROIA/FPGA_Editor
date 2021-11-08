

#SFML include
SFML_PATH = $$PWD/SFML

message( "--------- SFML.pri ---------- " )
message( "  Path: $$SFML_PATH " )

#SFML_PATH               = "$$ENGINE_PATH/extern/SFML"
SFML_DEBUG_DLL_BASE     = $$SFML_PATH\bin\Debug\lib    # Path to the dll's from SFML
SFML_RELEASE_DLL_BASE   = $$SFML_PATH\bin\Release\lib  # Path to the dll's from SFML


sfml_includePath  = "$$SFML_PATH/include"
sfml_binPath      = "$$SFML_PATH/bin"
sfml_libs_debug   = "$$sfml_binPath/Debug/lib/libsfml-audio-d.a" \
                    "$$sfml_binPath/Debug/lib/libsfml-graphics-d.a" \
                    "$$sfml_binPath/Debug/lib/libsfml-main-d.a" \
                    "$$sfml_binPath/Debug/lib/libsfml-network-d.a" \
                    "$$sfml_binPath/Debug/lib/libsfml-system-d.a" \
                    "$$sfml_binPath/Debug/lib/libsfml-window-d.a"

sfml_libs_release = "$$sfml_binPath/Release/lib/libsfml-audio.a" \
                    "$$sfml_binPath/Release/lib/libsfml-graphics.a" \
                    "$$sfml_binPath/Release/lib/libsfml-main.a" \
                    "$$sfml_binPath/Release/lib/libsfml-network.a" \
                    "$$sfml_binPath/Release/lib/libsfml-system.a" \
                    "$$sfml_binPath/Release/lib/libsfml-window.a"

# Copy dll's from SFML to the bin dir
CONFIG(debug, debug|release) {
    isEmpty(SFML_DEBUG_DLL_BASE) {
        SFML_DEBUG_DLL_BASE = $$SFML_PATH\bin\Debug\lib
    }
SFML_DEBUG_DLL_BASE ~= s,/,\\,g # Replace "/" with "\\"
message( "  Copy debug dll\'s: $$SFML_DEBUG_DLL_BASE" )
QMAKE_PRE_LINK = copy "$$SFML_DEBUG_DLL_BASE\sfml*.dll" "debug\sfml*.dll"
}else{
    isEmpty(SFML_RELEASE_DLL_BASE) {
        SFML_RELEASE_DLL_BASE = $$SFML_PATH\bin\Release\lib
    }
SFML_RELEASE_DLL_BASE ~= s,/,\\,g # Replace "/" with "\\"
message( "  Copy release dll\'s $$SFML_RELEASE_DLL_BASE" )
QMAKE_PRE_LINK = copy "$$SFML_RELEASE_DLL_BASE\sfml*.dll" "release\sfml*.dll"
}
message("  $$QMAKE_PRE_LINK")


CONFIG(release, debug|release): sfml_libs = $$sfml_libs_release
CONFIG(debug, debug|release):   sfml_libs = $$sfml_libs_debug
LIBS        += $$sfml_libs
INCLUDEPATH += $$sfml_includePath
DEPENDPATH  += $$sfml_includePath
# End SFML stuff

message( "----------------------------- " )
