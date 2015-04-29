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
#include <swiped/engine/engine.h>

#include <GLFW/glfw3.h>
#include <cassert>

#include <swiped/scene/scene.h>

namespace swiped
{

Engine& Engine::instance()
{
    static Engine engine;
    return engine;
}

void Engine::start()
{
    running_ = true;

    double time = glfwGetTime();
    double current_time = time;
    double time_delta = 0;

    while (running_ && !glfwWindowShouldClose(window_))
    {
        if (pending_scene_)
        {
            if (scene_)
            {
                scene_->cleanup();
                delete scene_;
            }
            pending_scene_->setup();

            scene_ = pending_scene_;
            pending_scene_ = 0;
        }

        current_time = glfwGetTime();
        time_delta = current_time - time;
        time = current_time;

        glfwPollEvents();
        scene_->handle_events();
        scene_->update(time_delta);
        scene_->render();
        
        glfwSwapBuffers(window_);
    }

    if (scene_)
    {
        scene_->cleanup();
        delete scene_;
    }

    if (pending_scene_)
    {
        delete pending_scene_;
    }
}

void Engine::set_scene(Scene* scene)
{
    pending_scene_ = scene;
}

GLFWwindow* Engine::get_window() const
{
  return window_;
}

void Engine::stop()
{
    running_ = false;
}

Engine::Engine()
  : running_(false),
    scene_(0),
    pending_scene_(0)
{
    int err = 0;
    
    err = glfwInit();
    assert(err && "error: failed to initialize glfw");

    // Make the window non resizable
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // Use 4 samples for multisampling
    glfwWindowHint(GLFW_SAMPLES,   4);

    // Warning: do not issue any GL call before the context 
    // is created in glfwOpenWindow! This would result in a
    // plain old segfault
    
    // Create Window and context
    window_ = glfwCreateWindow(1024,     // width
                               768,      // height
                               "Swiped", // Title
                               NULL,     // Monitor (NULL = windowed)
                               NULL);    // Shared context
    assert(err && "error: failed to open glfw window");

    //Make the window's context current
    glfwMakeContextCurrent(window_);

    // Enable multisampling for FSAA
    glEnable(GL_MULTISAMPLE);

    // Disable vertical synchronization
    glfwSwapInterval(0);
}

Engine::~Engine()
{
    glfwTerminate();
}

} // end of namespace swiped

