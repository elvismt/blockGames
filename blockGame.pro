TEMPLATE = app
TARGET = blockGame
CONFIG += qt
QT += gui widgets

SOURCES = \
          main.cpp \
    tetris.cpp \
    blockGame.cpp

HEADERS += \
    tetris.h \
    blockGame.h

RESOURCES += \
    images/images.qrc

