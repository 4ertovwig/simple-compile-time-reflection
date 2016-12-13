TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
QMAKE_CXXFLAGS += -std=c++14
QMAKE_LFLAGS +=  -std=c++14

SOURCES += main.cpp

HEADERS += \
    fields_reflection.hpp \
    function_reflection.hpp \
    help_functions_reflection.hpp \
    reflection.hpp
