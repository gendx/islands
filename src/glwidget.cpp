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

#include <cmath>
#include <GL/glu.h>

#include "glwidget.hpp"

GLWidget::GLWidget(unsigned int width, unsigned int height, const std::vector<double>& values, QWidget* parent) :
    QGLWidget(parent),
    mWidth(width),
    mHeight(height),
    mValues(values)
{
    xTrans = width / 2.0;
    yTrans = -std::sqrt(3) * height / 4.0;
    zoom = 100;
    rot = 0;
    mCtrl = false;
}

GLWidget::~GLWidget()
{
    if (mIslands)
        glDeleteLists(mIslands, 1);
}


// Initialize OpenGL.
void GLWidget::initializeGL()
{
    mIslands = makeIslands();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Repaint the view.
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(xTrans, yTrans + zoom * std::sin(rot * std::atan(1) / 45.0), zoom * std::cos(rot * std::atan(1) / 45.0),
              xTrans, yTrans, 0,
              0, std::cos(rot * std::atan(1) / 45.0), -std::sin(rot * std::atan(1) / 45.0));

    glCallList(mIslands);
    glPopMatrix();
}

// Resize the view.
void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, +5.0, -5.0, 5.0, 5.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40.0);
}


// Mouse operations.
void GLWidget::mousePressEvent(QMouseEvent* event)
{
    mMousePos = event->pos();
    event->accept();
}

// Translate the view.
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->x() - mMousePos.x();
    int dy = event->y() - mMousePos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        xTrans -= dx / 5;
        yTrans += dy / 5;
        updateGL();
    }

    mMousePos = event->pos();
    event->accept();
}

// Change zoom/rotation.
void GLWidget::wheelEvent(QWheelEvent* event)
{
    if (mCtrl)
    {
        rot += event->delta() / 30;

        while (rot < 0)
            rot += 360;
        while (rot > 360)
            rot -= 360;
    }
    else
    {
        double z = zoom - event->delta() / 6;
        if (z > 0)
            zoom = z;
    }

    updateGL();
    event->accept();
}

// Check if CTRL is pressed.
void GLWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control)
    {
        mCtrl = true;
        event->accept();
    }
    else
        event->ignore();
}

void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control)
    {
        mCtrl = false;
        event->accept();
    }
    else
        event->ignore();
}


// Create the mesh of islands.
GLuint GLWidget::makeIslands()
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    glBegin(GL_TRIANGLES);
    for (unsigned int x = 0 ; x < mWidth ; ++x)
    {
        for (unsigned int y = 0 ; y + 1 < mHeight ; ++y)
        {
            double val = (mValues[x * mHeight + y] - 1) / 5;
            if (val > 0)
                glColor3ub(
                            0xC0 * (1 - 1.25 * val) + 0x20 * 1.25 * val,
                            0xC0 * (1 - 1.25 * val) + 0xA0 * 1.25 * val,
                            0x78 * (1 - 1.25 * val) + 0x08 * 1.25 * val);
            else
                glColor3ub(
                            0xC0 * (4 * val + 1) - 0x00 * 4 * val,
                            0xC0 * (4 * val + 1) - 0x50 * 4 * val,
                            0x78 * (4 * val + 1) - 0xA0 * 4 * val);

            static const double msqrt3 = -std::sqrt(3);

            if (y % 2)
            {
                if (x + 1 < mWidth)
                {
                    glVertex3d(
                                x,
                                msqrt3 * y / (double)2,
                                mValues[x * mHeight + y]);
                    glVertex3d(
                                x + 1,
                                msqrt3 * y / (double)2,
                                mValues[(x + 1) * mHeight + y]);
                    glVertex3d(
                                x + 1 / (double)2,
                                msqrt3 * (y + 1) / (double)2,
                                mValues[x * mHeight + y + 1]);
                }
                if (x > 0)
                {
                    glVertex3d(
                                x,
                                msqrt3 * y / (double)2,
                                mValues[x * mHeight + y]);
                    glVertex3d(
                                x + 1 / (double)2,
                                msqrt3 * (y + 1) / (double)2,
                                mValues[x * mHeight + y + 1]);
                    glVertex3d(
                                x - 1 / (double)2,
                                msqrt3 * (y + 1) / (double)2,
                                mValues[(x - 1) * mHeight + y + 1]);
                }
            }
            else
            {
                if (x + 1 < mWidth)
                {
                    glVertex3d(
                                x + 1 / (double)2,
                                msqrt3 * y / (double)2,
                                mValues[x * mHeight + y]);
                    glVertex3d(
                                x + 3 / (double)2,
                                msqrt3 * y / (double)2,
                                mValues[(x + 1) * mHeight + y]);
                    glVertex3d(
                                x + 1,
                                msqrt3 * (y + 1) / (double)2,
                                mValues[(x + 1) * mHeight + y + 1]);


                    glVertex3d(
                                x + 1 / (double)2,
                                msqrt3 * y / (double)2,
                                mValues[x * mHeight + y]);
                    glVertex3d(
                                x + 1,
                                msqrt3 * (y + 1) / (double)2,
                                mValues[(x + 1) * mHeight + y + 1]);
                    glVertex3d(
                                x,
                                msqrt3 * (y + 1) / (double)2,
                                mValues[x * mHeight + y + 1]);
                }
            }
        }
    }
    glEnd();

    glEndList();
    return list;
}
