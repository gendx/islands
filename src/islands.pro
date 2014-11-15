#   Islands - an algorithm to generate a random map of islands.
#   Copyright (C) 2014  G. Endignoux
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.txt

TARGET = islands
TEMPLATE = app

QT += opengl
LIBS += -lGLU

QMAKE_CXXFLAGS += --std=c++11

SOURCES += main.cpp \
    glwidget.cpp \
    population.cpp

HEADERS  += \
    glwidget.hpp \
    population.hpp
