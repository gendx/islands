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

#include "population.hpp"
#include <cmath>
#include <ctime>

std::mt19937& Population::generator = Population::initGenerator();

// Create a honeycomb and cache the probabilities for a given period (spatial size of attraction) and amplitude (intensity of attraction).
Population::Population(unsigned int width, unsigned int height, double period, double amplitude) :
    mWidth(width),
    mHeight(height),
    mCount(width * height),
    mPeriod(period),
    mAmplitude(amplitude),
    mProbas(2 * mCount - 1, 0),
    mValues(mCount, 1)
{
    // Each cell has probability 1 (we don't need to normalize by the total probability to 1).
    for (unsigned int j = mCount - 1 ; j < 2 * mCount - 1 ; ++j)
        this->addProba(j, 1);

    // Estimate delta so that the cache keeps track of all probabilities above epsilon = 0.001
    mDelta = std::ceil(std::sqrt(mPeriod * std::log(mAmplitude / (mPeriod * .001))));

    // Compute the Gaussian distribution on a square of side delta + 1
    mCache.reserve((mDelta + 1) * (mDelta + 1));
    for (unsigned int x = 0 ; x <= mDelta ; ++x)
        for (unsigned int y = 1 ; y <= mDelta + 1 ; ++y)
            mCache.push_back(mAmplitude * std::exp(-this->distance(std::make_pair(0, 1), std::make_pair(x, y)) / mPeriod) / mPeriod);
}


// Get a distribution of probabilities normalized by the maximum value.
std::vector<double> Population::getProbas()
{
    double max = 0.;
    for (unsigned int i = 0 ; i < mCount ; ++i)
        if (mValues[i] > max)
            max = mValues[i];

    std::vector<double> result = mValues;
    for (unsigned int i = 0 ; i < mCount ; ++i)
        result[i] *= 5 / max;
    return result;
}

// Add "diff" to the score of cell number j.
void Population::addProba(unsigned int j, double diff)
{
    while (j)
    {
        mProbas[j] += diff;
        j = (j - 1) / 2;
    }
    mProbas[0] += diff;
}

// Add a particle in the population, according to the current distribution of probabilities.
void Population::addParticle()
{
    unsigned int pos = this->pickPosition();
    Point position = std::make_pair((pos + 1 - mCount) / mHeight, (pos + 1) % mHeight);

    unsigned int xmin = std::max(0, int(position.first - mDelta));
    unsigned int xmax = std::min(mWidth, position.first + mDelta + 1);
    unsigned int ymin = std::max(0, int(position.second - mDelta));
    unsigned int ymax = std::min(mHeight, position.second + mDelta + 1);

    // Loop on all neighbors.
    for (unsigned int x = xmin ; x < xmax ; ++x)
    {
        for (unsigned int y = ymin ; y < ymax ; ++y)
        {
            unsigned int j = mCount - 1 + x * mHeight + y;

            unsigned int dy = abs(y - position.second);
            unsigned int dx = abs(x - position.first);

            if ((dy % 2) && ((y % 2 && (x > position.first)) || (y % 2 == 0 && (position.first > x))))
                --dx;

            // Update probability.
            if (pos == j)
                this->addProba(j, -mProbas[j]);
            else if (mProbas[j] != 0)
                this->addProba(j, mCache[dx * (mDelta + 1) + dy]);

            mValues[j + 1 - mCount] += mCache[dx * (mDelta + 1) + dy];
        }
    }
}


// Choose a random cell according to probabilities.
unsigned int Population::pickPosition()
{
    unsigned int j = 0;
    // We use an internal binary tree to pick up the position in O(log(n)).
    while (2 * j + 1 < 2 * mCount - 1)
    {
        double max = mProbas[j];
        std::uniform_real_distribution<> distrib(0., max);
        double value = distrib(Population::generator);

        j = 2 * j + 1;
        if (value > mProbas[j])
            ++j;
    }

    return j;
}


// Get the euclidian distance between points on the honeycomb.
double Population::distance(Point p1, Point p2)
{
    static const double C = std::sqrt(3) / 2;

    double x1 = (p1.first + 1);
    if (p1.second % 2)
        x1 -= .5;
    double y1 = (p1.second + 1) * C;

    double x2 = (p2.first + 1);
    if (p2.second % 2)
        x2 -= .5;
    double y2 = (p2.second + 1) * C;

    double dx = x1 - x2;
    double dy = y1 - y2;
    return dx * dx + dy * dy;
}


// Init the random number generator with current time.
std::mt19937& Population::initGenerator()
{
    static std::mt19937 generator;
    generator.seed(std::time(0));
    return generator;
}
