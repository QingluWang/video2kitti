QT       += core gui
# C++ 11
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    converter.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    converter.h \
    mainwindow.h
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so \
    /usr/lib/x86_64-linux-gnu/libopencv_core.so \
    /usr/lib/x86_64-linux-gnu/libopencv_features2d.so \
    /usr/lib/x86_64-linux-gnu/libopencv_flann.so \
    /usr/lib/x86_64-linux-gnu/libopencv_highgui.so \
    /usr/local/lib/libopencv_imgcodecs.so \
    /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so \
    /usr/lib/x86_64-linux-gnu/libopencv_ml.so \
    /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so \
    /usr/lib/x86_64-linux-gnu/libopencv_photo.so \
    /usr/lib/x86_64-linux-gnu/libopencv_stitching.so \
    /usr/lib/x86_64-linux-gnu/libopencv_superres.so \
    /usr/local/lib/libopencv_videoio.so \
    /usr/lib/x86_64-linux-gnu/libopencv_video.so \
    /usr/lib/x86_64-linux-gnu/libopencv_videostab.so \
    /usr/lib/x86_64-linux-gnu/libopencv_contrib.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
