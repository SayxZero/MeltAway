QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

LIBS += -lopengl32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    layer.cpp \
    main.cpp \
    meltaway.cpp \
    model.cpp \
    subclasses.cpp \
    widgetgl3d.cpp

HEADERS += \
    layer.h \
    meltaway.h \
    model.h \
    subclasses.h \
    widgetgl3d.h

FORMS += \
    meltaway.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
