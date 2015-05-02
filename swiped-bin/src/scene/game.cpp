/*
Copyright (C) 2014 Aurelien Vallee

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <swiped/bin/scene/game.h>

#include <swiped/engine/engine.h>
#include <swiped/engine/static_text.h>

#include <swiped/bin/scene/menu.h>

#include <sstream>

GameScene::GameScene(swiped::Engine& engine)
    : swiped::Scene(engine)
{
}

bool GameScene::setup()
{
    // Clear values
    glClearColor(0.25, 0.5, 0.75, 0);
    glClearDepth(0);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,                                     // Left
            swiped::Engine::instance().get_width(),  // Right
            0.0,                                     // Bottom
            swiped::Engine::instance().get_height(), // Top
            -1.0,                                    // Near
            1.0);                                    // Far

    // Setup modelview matrix
    glMatrixMode(GL_MODELVIEW);

    //...

    return true;
}

bool GameScene::cleanup()
{
    //...

    return true;
}

void GameScene::key_pressed(int key, int action)
{
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
    {
        engine_.set_scene(new MenuScene(engine_));
    }
}

void GameScene::update(double /*time_delta*/)
{
}

void GameScene::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Control points
    GLfloat control_points[][3] = {
        {512,   0, 0},
        {200, 400, 0},
        {900, 600, 0},
        {512, 768, 0},
    };
    static const unsigned int control_points_count = sizeof(control_points)/sizeof(control_points[0]);

    // Draw control points
    glPointSize(5);
    glBegin(GL_POINTS);
        for (unsigned int i=0; i<control_points_count; ++i)
            glVertex2fv(control_points[i]);
    glEnd();

    // Draw text near control points
    for (unsigned int i=0; i<control_points_count; ++i)
    {
        swiped::StaticText txt;
        txt.set_position(control_points[i][0]+3, control_points[i][1]+7);
        std::ostringstream oss;
        oss << i;
        txt.set_text(oss.str());
        txt.render();
    }

    // Draw simple lines
    glColor4f(1, 1, 1, 0.2);
    glBegin(GL_LINE_STRIP);
        for (unsigned int i=0; i<control_points_count; ++i)
            glVertex2fv(control_points[i]);
    glEnd();
    glColor4f(1, 1, 1, 1);

    // Draw bezier curve
    glLineWidth(2);
    glColor4f(1, 1, 1, 0.7);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, control_points_count, &control_points[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);
        for (unsigned int i=0; i<=50; ++i)
            glEvalCoord1f((float)i/50.);
    glEnd();
    glColor4f(1, 1, 1, 1);
}

