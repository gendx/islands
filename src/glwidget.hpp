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

#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP

#include <QWidget>
#include <QtOpenGL/QtOpenGL>

// Widget to display the heightmap of islands with opengl.
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(unsigned int width, unsigned int height, const std::vector<double>& values, QWidget* parent = 0);
    ~GLWidget();

private:
    // opengl operations
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    // Mouse & keybord control
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    // create the heightmap in opengl
    GLuint makeIslands();

    // Heightmap.
    unsigned int mWidth;
    unsigned int mHeight;
    std::vector<double> mValues;

    // Mesh of islands.
    GLuint mIslands;

    // View.
    double xTrans;
    double yTrans;
    double zoom;
    double rot;

    // Whether CTRL is pressed on keyboard.
    bool mCtrl;
    // Position of mouse.
    QPoint mMousePos;
};

#endif // GLWIDGET_HPP
