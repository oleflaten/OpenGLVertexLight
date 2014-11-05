QT += opengl

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    main.cpp \
    openglwindow.cpp \
    trianglewindow.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
INSTALLS += target

HEADERS += \
    trianglewindow.h \
    openglwindow.h

RESOURCES += \
    shaders.qrc
