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
#include <swiped/swiped.h>
#include <swiped/scene/scene.h>

#include <AL/al.h>

class ChessboardEffect
{
public:
    ChessboardEffect();
    ~ChessboardEffect();

private: // Non copyable
    ChessboardEffect(const ChessboardEffect&);
    ChessboardEffect& operator=(const ChessboardEffect&);

public:
    void setup(unsigned int width,
               unsigned int height,
               unsigned int rows,
               unsigned int columns,
               unsigned int spacing,
               unsigned int count,
               float color_r_from,
               float color_g_from,
               float color_b_from,
               float color_r_to,
               float color_g_to,
               float color_b_to);
    void cleanup();
    void update();
    void render();

    float&       at(unsigned int x, unsigned int y);
    const float& at(unsigned int x, unsigned int y) const;

    float&       vat(unsigned int x, unsigned int y);
    const float& vat(unsigned int x, unsigned int y) const;

private:
    unsigned int width_;
    unsigned int height_;
    unsigned int rows_;
    unsigned int columns_;
    unsigned int spacing_;
    unsigned int count_;
    float color_r_from_;
    float color_g_from_;
    float color_b_from_;
    float color_r_to_;
    float color_g_to_;
    float color_b_to_;

    float* buffer_;
    float* bufferVelocity_;
};

class MenuScene : public swiped::Scene
{
public:
    MenuScene(swiped::Engine& engine);

public:
    virtual bool setup();
    virtual bool cleanup();

    virtual void key_pressed(int key, int action);
    virtual void update(double /*time_delta*/);

    virtual void render();

private:
    // Textures
    GLuint text_title;
    GLuint text_play;
    GLuint text_options;
    GLuint text_credits;
    GLuint text_quit;

    // Sounds
    ALuint snd_menu_;

    // Menu
    unsigned int current_menu_;

    ChessboardEffect effect_chessboard_;
};

