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
#ifndef SWIPED_ENGINE_H
#define SWIPED_ENGINE_H

#include <swiped/swiped.h>

namespace swiped
{

class Scene;

/*
 * This is the main entry point of the game. The Engine class
 * manages the windowing subsystem along with the setup of the
 * event loop.
 * This class is a singleton, if you whish to use it, just use
 * the instance() method.
 */
class SWIPED_EXPORT Engine
{
public:
    static Engine& instance();

public:
    void start();
    void stop();

public:
    void set_scene(Scene* scene);

private:
    Engine();
    ~Engine();

private:
    bool running_;
    Scene* scene_;
    Scene* pending_scene_;
};

} // end of namespace swiped

#endif // SWIPED_ENGINE_H

