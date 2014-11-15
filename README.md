# Islands

This project is a demonstration of an algorithm to randomly generate islands on a map.


## Installation

In order to build this software, you will need to install :
* A C++ compiler supporting the C++11 standard such as GCC >= 4.8.
* Qt4, available at [qt-project](http://qt-project.org/downloads). Qt5 was not tested yet, but it should be compatible.
* OpenGL and GLU libraries.


## Usage

The demo displays the result in a window. You can navigate through the map using the following commands :
* mouse click and drag : translation on the map
* mouse wheel : zoom in and out
* CTRL + mouse wheel : rotation of the map

Feel free to test several sets of parameters in `main.cpp` !


## Algorithm

To generate islands, the following method was used.
* The map can be seen as a honeycomb where each cell is given a score (initially one). We fill part of these cells with particles at random.
* Whenever we add a particle in a cell, it modifies the scores of the neighboring cells, which is proportional to the probability of the apparition of a new particle. Thus, particles tend to stay close together.
* At the end of this process, we obtain islands made of groups of particles. We convert the scores into a heightmap which is displayed on screen.

This method can be adapted to create a forest (if we see particles as trees), or random ressources on a map.


## License

*Copyright (C) 2014  G. Endignoux*

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see [www.gnu.org/licenses/gpl-3.0.txt](http://www.gnu.org/licenses/gpl-3.0.txt).

