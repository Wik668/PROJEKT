TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += gui

SOURCES += \
        AnimatedSprite.cpp \
        Button.cpp \
        Medkit.cpp \
        Menu.cpp \
        Slime.cpp \
        bullet.cpp \
        main.cpp \
        ninja.cpp \
        zombie.cpp
INCLUDEPATH += "C:/SFML-2.5.1/include"
LIBS += -L"C:/SFML-2.5.1/lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}

HEADERS += \
    AnimatedSprite.h \
    Button.h \
    Medkit.h \
    Menu.h \
    Slime.h \
    SlimeProjectile.h \
    bullet.h \
    ninja.h \
    utils.h \
    zombie.h
