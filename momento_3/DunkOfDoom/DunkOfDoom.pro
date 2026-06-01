QT       += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ballsprites.cpp \
    boss_sprites.cpp \
    hud_sprites.cpp \
    lavasprites.cpp \
    level.cpp \
    level_2.cpp \
    main.cpp \
    mainwindow.cpp \
    ball.cpp \
    character.cpp \
    player.cpp \
    enemy.cpp \
    boss.cpp \
    obstacle.cpp \
    player_sprites.cpp \
    powerupsprites.cpp \
    trap.cpp \
    lava.cpp \
    powerup.cpp \
    trapsprites.cpp

HEADERS += \
    ballsprites.h \
    boss_sprites.h \
    config.h \
    hud_sprites.h \
    lavasprites.h \
    level.h \
    level_2.h \
    mainwindow.h \
    ball.h \
    character.h \
    player.h \
    enemy.h \
    boss.h \
    obstacle.h \
    player_sprites.h \
    powerupsprites.h \
    trap.h \
    lava.h \
    powerup.h \
    trapsprites.h \
    zona.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32 {
    DESTDIR = $$OUT_PWD

    assets.path  = $$DESTDIR/assets
    assets.files = $$PWD/assets/*
    INSTALLS += assets

    # Copiar en cada build
    copyassets.commands = xcopy /E /I /Y \"$$shell_path($$PWD/assets)\" \"$$shell_path($$DESTDIR/assets)\"
    first.depends = $(first) copyassets
    export(first.depends)
    export(copyassets.commands)
    QMAKE_EXTRA_TARGETS += first copyassets
}
