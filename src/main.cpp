/*
    Islands - an algorithm to generate a random map of islands.
    Copyright (C) 2014  G. Endignoux

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.txt
*/

#include <QtGui/QApplication>
#include "population.hpp"
#include "glwidget.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
    unsigned int width = 520, height = 680;
    double period = 10, amplitude = 10000;
    unsigned int count = 10000;
    //*/
    //*
    unsigned int width = 520, height = 680;
    double period = 3, amplitude = 1000;
    unsigned int count = 100000;
    //*/
    /*
    unsigned int width = 1040, height = 1360;
    double period = 10, amplitude = 10000;
    unsigned int count = 500000;
    //*/

    // Generate a population.
    Population population(width, height, period, amplitude);
    for (unsigned int i = 0 ; i < count ; ++i)
        population.addParticle();

    // Display it !
    GLWidget glw(width, height, population.getProbas());
    glw.show();

    return a.exec();
}
