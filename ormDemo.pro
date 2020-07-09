#-------------------------------------------------
#
# Project created by QtCreator 2020-06-30T22:46:06
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ormDemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#通过它可以知道项目是否正在编译
DEFINES += _BUILDING_QX_BLOG

# 预编译头文件
!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
    PRECOMPILED_HEADER = precompiled.h
}

# QxOrm 库相关配置
INCLUDEPATH += $$PWD/QxOrm/include
LIBS += -L$$PWD/QxOrm/lib

# 设置生成的目标名称、添加依赖库
CONFIG(debug, debug|release) {
    LIBS += -lQxOrmd
} else {
    LIBS += -lQxOrm
}



SOURCES += \
        main.cpp \
        dialog.cpp \
    author.cpp \
    category.cpp \
    blog.cpp \
    comment.cpp

HEADERS += \
        dialog.h \
    export.h \
    precompiled.h \
    author.h \
    category.h \
    blog.h \
    comment.h

FORMS += \
        dialog.ui
