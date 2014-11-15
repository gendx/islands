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

#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <vector>
#include <random>


typedef std::pair<unsigned int, unsigned int> Point;

// Population of "particles" on a honeycomb.
// Each cell of the comb has a score which defines the probability of a particle to come into it.
// Each particle modifies the scores of neighbouring cells, so that new particles are more likely to come in the nearby.
// More precisely, each particle generates a Gaussian distribution around it, that adds to the previous probabilities.
// The resulting population of particles is a set of islands, because particles tend to stay close together.
class Population
{
public:
    // Create a honeycomb and cache the probabilities for a given period (spatial size of attraction) and amplitude (intensity of attraction).
    Population(unsigned int width, unsigned int height, double period, double amplitude);

    // Get a heightmap of probabilities, which can be shown as islands in the sea.
    std::vector<double> getProbas();

    // Add a particle in the population, according to the current distribution of probabilities.
    void addParticle();

private:
    // Add "diff" to the score of cell number j.
    void addProba(unsigned int j, double diff);

    // Choose a random cell according to probabilities.
    unsigned int pickPosition();
    // Get the euclidian distance between points on the honeycomb.
    double distance(Point p1, Point p2);

    // Random number generator.
    static std::mt19937& generator;
    static std::mt19937& initGenerator();

    // Parameters.
    unsigned int mWidth;
    unsigned int mHeight;
    unsigned int mCount;
    double mPeriod;
    double mAmplitude;

    // Caching of the Gaussian distribution.
    // At each step, we only update cells in a neighbourhood of size delta, because a Gaussian goes fast to zero.
    // This trick saves a lot of time !
    unsigned int mDelta;
    std::vector<double> mCache;

    // Current distribution of probabilities.
    std::vector<double> mProbas;
    std::vector<double> mValues;
};

#endif // POPULATION_HPP
