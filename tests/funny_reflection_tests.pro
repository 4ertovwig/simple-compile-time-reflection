TEMPLATE = app
TARGET = funny_reflection_tests
QMAKE_CXXFLAGS += -std=c++14
QMAKE_LFLAGS +=  -std=c++14
CONFIG += console c++14

LIBS += -lboost_unit_test_framework

HEADERS += \
        ../funny_reflection/help_functions_reflection.hpp \
    	../funny_reflection/fields_reflection.hpp \
	../funny_reflection/function_reflection.hpp \
	../funny_reflection/reflection.hpp

SOURCES += \
        main.cpp \
        test_reflection.cpp
