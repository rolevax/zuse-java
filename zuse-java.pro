TEMPLATE = app

QT += qml quick
QT += widgets

CONFIG += c++11

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(release, debug|release): DEFINES += NDEBUG

SOURCES += main.cpp \
    core/doc.cpp \
    core/token.cpp \
    gui/pdoc.cpp \
    ast/ast.cpp \
    ast/listast.cpp \
    ast/scalarast.cpp \
    ast/rootast.cpp \
    ast/parser.cpp \
    ast/bison.cc \
    ast/lex.cpp \
    core/bonetoken.cpp \
    core/fleshtoken.cpp \
    core/soultoken.cpp \
    core/tokens.cpp \
    core/hammer.cpp \
    mode/menumode.cpp \
    mode/stringinputmode.cpp \
    mode/numberinputmode.cpp \
    ast/internalast.cpp \
    ast/fixsizeast.cpp \
    mode/identinputmode.cpp \
    ast/boplistast.cpp \
    mode/listinputmode.cpp \
    core/keycode.cpp \
    mode/tilexmode.cpp \
    mode/fixsizeinputmode.cpp \
    mode/macrohandler.cpp \
    mode/mogglemode.cpp \
    mode/normalmode.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    core/doc.h \
    core/token.h \
    ast/ast.h \
    ast/listast.h \
    gui/pdoc.h \
    ast/scalarast.h \
    ast/rootast.h \
    core/bonetoken.h \
    core/fleshtoken.h \
    core/soultoken.h \
    core/tokens.h \
    core/hammer.h \
    core/tokensobserver.h \
    mode/mode.h \
    mode/menumode.h \
    mode/stringinputmode.h \
    mode/numberinputmode.h \
    core/editabledoc.h \
    ast/internalast.h \
    ast/fixsizeast.h \
    mode/identinputmode.h \
    ast/boplistast.h \
    mode/listinputmode.h \
    core/keycode.h \
    mode/tilexmode.h \
    mode/fixsizeinputmode.h \
    mode/macrohandler.h \
    ast/modifiers.h \
    mode/mogglemode.h \
    mode/normalmode.h

