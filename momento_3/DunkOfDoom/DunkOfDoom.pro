QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ball.cpp \
    character.cpp \
    player.cpp \
    enemy.cpp \
    boss.cpp \
    obstacle.cpp \
    trap.cpp \
    lava.cpp \
    powerup.cpp

HEADERS += \
    mainwindow.h \
    ball.h \
    character.h \
    player.h \
    enemy.h \
    boss.h \
    obstacle.h \
    trap.h \
    lava.h \
    powerup.h \
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
