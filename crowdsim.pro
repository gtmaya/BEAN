include(includes.pri)

TARGET =        Crowdsim

OBJECTS_DIR =   obj

MOC_DIR =       moc

INCLUDEPATH +=  ./include

DESTDIR =       ./

HEADERS +=      include/renderscene.h \
                include/usercamera.h \
                include/shaderhandler.h \
                include/peep.h \
                include/crowdsim.h \
                include/map.h

SOURCES +=      src/main.cpp \
                src/usercamera.cpp \
                src/renderscene.cpp \
                src/shaderhandler.cpp \
                src/crowdsim.cpp \
                src/peep.cpp

DISTFILES +=    $OTHER_FILES \
                shaders/env_f.glsl \
                shaders/env_v.glsl \
                shaders/phong_f.glsl \
                shaders/phong_v.glsl \
                shaders/blit_v.glsl \
                shaders/blit_f.glsl \
                shaders/beckmann_v.glsl \
                shaders/beckmann_f.glsl \
                shaders/taa_f.glsl \
                shaders/taa_v.glsl \
                shaders/msaa_f.glsl \
                shaders/msaa_v.glsl \
                src/mapreader.py \
                src/testMap.cpp

QMAKE_CXXFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

LIBS+=$$system(sdl2-config  --libs)
message(output from sdl2-config --libs added to LIB=$$LIBS)
