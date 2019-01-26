TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -static
QMAKE_MAC_SDK = macosx10.13

SOURCES += main.cpp \
    fichiers/code_files/sources/tools.cpp \
    fichiers/code_files/sources/database.cpp \
    fichiers/code_files/sources/sequence_managing.cpp \
    fichiers/code_files/sources/grid_managing.cpp \
    fichiers/code_files/sources/boolean_tests.cpp \
    fichiers/code_files/sources/player_actions.cpp \
    fichiers/code_files/sources/bot.cpp \
    fichiers/code_files/sources/game.cpp \

HEADERS += \
    fichiers/code_files/headers/tools.h \
    fichiers/code_files/headers/database.h \
    fichiers/code_files/headers/sequence_managing.h \
    fichiers/code_files/headers/grid_managing.h \
    fichiers/code_files/headers/boolean_tests.h \
    fichiers/code_files/headers/player_actions.h \
    fichiers/code_files/headers/bot.h \
    fichiers/code_files/headers/struct.h \
    fichiers/code_files/headers/type.h \
    fichiers/code_files/headers/const.h
