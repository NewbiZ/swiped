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
#include <swiped/bin/scene/notimplemented.h>

#include <swiped/engine/engine.h>

#include <swiped/bin/scene/menu.h>

NotImplementedScene::NotImplementedScene(swiped::Engine& engine)
    : swiped::Scene(engine)
{
}

bool NotImplementedScene::setup()
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

    text_.set_text("Not Implemented");
    text_.set_position(swiped::Engine::instance().get_width()/2-60,
                       swiped::Engine::instance().get_height()/2);

    return true;
}

bool NotImplementedScene::cleanup()
{
    return true;
}

void NotImplementedScene::key_pressed(int key, int action)
{
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
    {
        engine_.set_scene(new MenuScene(engine_));
    }
}

void NotImplementedScene::update(double /*time_delta*/)
{
}

void NotImplementedScene::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    text_.render();
}

