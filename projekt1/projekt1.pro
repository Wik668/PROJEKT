TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += gui

SOURCES += \
        Ammo.cpp \
        AnimatedSprite.cpp \
        Boss.cpp \
        Button.cpp \
        EndGameMenu.cpp \
        Medkit.cpp \
        Menu.cpp \
        Slime.cpp \
        bullet.cpp \
        main.cpp \
        ninja.cpp \
        syringe.cpp \
        zombie.cpp
INCLUDEPATH += "C:/SFML-2.5.1/include"
LIBS += -L"C:/SFML-2.5.1/lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}

HEADERS += \
    Ammo.h \
    AnimatedSprite.h \
    Boss.h \
    Button.h \
    EndGameMenu.h \
    Fireball.h \
    Medkit.h \
    Menu.h \
    Slime.h \
    SlimeProjectile.h \
    bullet.h \
    ninja.h \
    syringe.h \
    syringe.h \
    utils.h \
    zombie.h
