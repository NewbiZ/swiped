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
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include <AL/al.h>
#include <AL/alut.h>

#include <swiped/swiped.h>
#include <swiped/engine/engine.h>
#include <swiped/scene/scene.h>
#include <swiped/loader/tga.h>
#include <swiped/engine/static_text.h>

unsigned int* current_menu = 0;

GLuint load_texture(const std::string& filename)
{
    GLuint texture;
    swiped::TextureInfo texture_info;

    swiped::load_tga(filename, texture_info);

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(texture_info.target,
                 texture_info.level,
                 texture_info.internalFormat,
                 texture_info.width,
                 texture_info.height,
                 0,
                 texture_info.format,
                 texture_info.type,
                 texture_info.data);

    delete [] texture_info.data;

    return texture;
}

void menu_key_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key==GLFW_KEY_DOWN && action==GLFW_PRESS)
    {
        *current_menu += 1;
        *current_menu %= 4;
        alSourcePlay(1);
    }
    else if (key==GLFW_KEY_UP && action==GLFW_PRESS)
    {
        *current_menu -= 1;
        *current_menu %= 4;
        alSourcePlay(1);
    }
}

class ChessboardEffect
{
public:
    ChessboardEffect()
    {
    }

    ~ChessboardEffect()
    {
    }

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
               float color_b_to)
    {
        width_  = width;
        height_ = height;
        rows_    = rows;
        columns_ = columns;
        spacing_ = spacing;
        count_   = count;
        color_r_from_ = color_r_from;
        color_g_from_ = color_g_from;
        color_b_from_ = color_b_from;
        color_r_to_ = color_r_to;
        color_g_to_ = color_g_to;
        color_b_to_ = color_b_to;

        buffer_ = new float[rows_ * columns_];
        bufferVelocity_ = new float[rows_ * columns_];
        for (unsigned int i=0; i<rows_*columns_; ++i)
        {
            buffer_[i] = 0;
            bufferVelocity_[i] = 0;
        }

        while (count--)
        {
            unsigned int x = rand() % columns_;
            unsigned int y = rand() % rows_;
            vat(x, y) = 0.001;
            at(x, y) = rand() / (float)RAND_MAX;
        }
    }

    void cleanup()
    {
        delete [] buffer_;
        delete [] bufferVelocity_;
    }

    void update()
    {
        // Count currently moving points
        unsigned int count = 0;
        for (unsigned int i=0; i<rows_*columns_; ++i)
        {
            if (bufferVelocity_[i] != 0)
                ++count;
        }
        // Keep up the count of moving points
        while (count < count_)
        {
            unsigned int x = rand() % columns_;
            unsigned int y = rand() % rows_;
            vat(x, y) = 0.001;
            ++count;
        }
        // Move the points
        for (unsigned int i=0; i<rows_*columns_; ++i)
        {
            buffer_[i] += bufferVelocity_[i];
            if (buffer_[i]>=1.0)
                bufferVelocity_[i] = -0.001;
        }
        // Reset moved points
        for (unsigned int i=0; i<rows_*columns_; ++i)
        {
            if (buffer_[i]<=0 && bufferVelocity_[i]<0)
            {
                bufferVelocity_[i] = 0;
                buffer_[i] = 0;
            }
        }
    }

    void render()
    {
        glDisable(GL_BLEND);
        for (unsigned int x=0; x<columns_; ++x)
        {
            for (unsigned int y=0; y<rows_; ++y)
            {
                if (at(x, y)!=0)
                {
                    float r = (float)color_r_from_ + (float)(color_r_to_ - color_r_from_)*(float)at(x,y);
                    float g = (float)color_g_from_ + (float)(color_g_to_ - color_g_from_)*(float)at(x,y);
                    float b = (float)color_b_from_ + (float)(color_b_to_ - color_b_from_)*(float)at(x,y);
                    glColor3f(r, g, b);
                    glBegin(GL_QUADS);
                        glVertex2i(width_/columns_ * x, height_/rows_ * y);
                        glVertex2i(width_/columns_ * x + width_/columns_, height_/rows_ * y);
                        glVertex2i(width_/columns_ * x + width_/columns_, height_/rows_ * y + height_/rows_);
                        glVertex2i(width_/columns_ * x, height_/rows_ * y + height_/rows_);
                    glEnd();
                }
            }
        }
        glColor3f(1., 1., 1.);
        glEnable(GL_BLEND);
    }

    float& at(unsigned int x, unsigned int y)
    {
      return buffer_[y * columns_ + x];
    }

    const float& at(unsigned int x, unsigned int y) const
    {
      return buffer_[y * columns_ + x];
    }

    float& vat(unsigned int x, unsigned int y)
    {
      return bufferVelocity_[y * columns_ + x];
    }

    const float& vat(unsigned int x, unsigned int y) const
    {
      return bufferVelocity_[y * columns_ + x];
    }

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
    MenuScene(swiped::Engine& engine)
        : swiped::Scene(engine)
    {
    }

public:
    virtual bool setup()
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
        glOrtho (0.0,                                     // Left
                 swiped::Engine::instance().get_width(),  // Right
                 0.0,                                     // Bottom
                 swiped::Engine::instance().get_height(), // Top
                 -1.0,                                    // Near
                 1.0);                                    // Far

        // Setup modelview matrix
        glMatrixMode(GL_MODELVIEW);

        text_title   = load_texture("install/bin/data/text_title.tga");
        text_play    = load_texture("install/bin/data/text_play_border.tga");
        text_options = load_texture("install/bin/data/text_options_border.tga");
        text_credits = load_texture("install/bin/data/text_credits_border.tga");
        text_quit    = load_texture("install/bin/data/text_quit_border.tga");

        current_menu_ = 0;

        current_menu = &current_menu_;
        glfwSetKeyCallback(swiped::Engine::instance().get_window(), menu_key_callback);

        alutInit(0, NULL);

        ALuint source;
        ALuint source2;

        alGenSources(1, &source);
        alGenSources(1, &source2);

        ALuint buffer = alutCreateBufferFromFile("install/bin/data/snd_menu.wav");
        ALuint buffer2 = alutCreateBufferFromFile("install/bin/data/snd_background.wav");

        alSourcei(source, AL_BUFFER, buffer);
        alSourcei(source2, AL_BUFFER, buffer2);

        snd_menu_ = source;

        alSourcePlay(2);

        effect_chessboard_.setup(1024,
                                 768,
                                 20,
                                 20,
                                 0,
                                 50,
                                 0.25,
                                 0.50,
                                 0.75,
                                 0.35,
                                 0.65,
                                 0.85);

        return true;
    }

    virtual bool cleanup()
    {
        glDeleteTextures(1, &text_title);
        glDeleteTextures(1, &text_play);
        glDeleteTextures(1, &text_options);
        glDeleteTextures(1, &text_credits);
        glDeleteTextures(1, &text_quit);

        effect_chessboard_.cleanup();

        return true;
    }

    virtual void handle_events()
    {
      GLFWwindow* window = swiped::Engine::instance().get_window();
        // Exit when we press Escape
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
            engine_.stop();
    }

    virtual void update(double /*time_delta*/)
    {
        effect_chessboard_.update();
    }

    virtual void render()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        effect_chessboard_.render();

        unsigned int title_width  = 512;
        unsigned int title_height = 128;
        unsigned int title_x_offset = swiped::Engine::instance().get_width()/2 - title_width/2;
        unsigned int title_y_offset = swiped::Engine::instance().get_height() - title_height - 20;

        unsigned int button_width  = 256;
        unsigned int button_height = 64;
        unsigned int button_x_offset = swiped::Engine::instance().get_width()/2 - button_width/2;
        unsigned int button_y_offset = swiped::Engine::instance().get_height() - button_height - 250;
        unsigned int button_y_spacing = button_height + 20;

        // Render "Swiped"
        glBindTexture(GL_TEXTURE_2D, text_title);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0, 0.0);
          glVertex2i(title_x_offset, title_y_offset);

          glTexCoord2f(1.0, 0.0);
          glVertex2i(title_x_offset + title_width, title_y_offset);

          glTexCoord2f(1.0, 1.0);
          glVertex2i(title_x_offset + title_width, title_y_offset + title_height);

          glTexCoord2f(0.0, 1.0);
          glVertex2i(title_x_offset, title_y_offset + title_height);
        glEnd();

        // Render "Play"
        button_y_offset -= button_y_spacing;
        glBindTexture(GL_TEXTURE_2D, text_play);

        if (current_menu_ == 0)
        {
          glColor4f(1., 1., 1., 0.5);
          glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex2i(5+button_x_offset, button_y_offset-5);

            glTexCoord2f(1.0, 0.0);
            glVertex2i(5+button_x_offset + button_width, button_y_offset-5);

            glTexCoord2f(1.0, 1.0);
            glVertex2i(5+button_x_offset + button_width, button_y_offset + button_height-5);

            glTexCoord2f(0.0, 1.0);
            glVertex2i(5+button_x_offset, button_y_offset + button_height-5);
          glEnd();
          glColor4f(1., 1., 1., 1.);
        }

        glBegin(GL_QUADS);
          glTexCoord2f(0.0, 0.0);
          glVertex2i(button_x_offset, button_y_offset);

          glTexCoord2f(1.0, 0.0);
          glVertex2i(button_x_offset + button_width, button_y_offset);

          glTexCoord2f(1.0, 1.0);
          glVertex2i(button_x_offset + button_width, button_y_offset + button_height);

          glTexCoord2f(0.0, 1.0);
          glVertex2i(button_x_offset, button_y_offset + button_height);
        glEnd();


        // Render "Options"
        button_y_offset -= button_y_spacing;
        glBindTexture(GL_TEXTURE_2D, text_options);

        if (current_menu_ == 1)
        {
          glColor4f(1., 1., 1., 0.5);
          glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex2i(5+button_x_offset, button_y_offset-5);

            glTexCoord2f(1.0, 0.0);
            glVertex2i(5+button_x_offset + button_width, button_y_offset-5);

            glTexCoord2f(1.0, 1.0);
            glVertex2i(5+button_x_offset + button_width, button_y_offset + button_height-5);

            glTexCoord2f(0.0, 1.0);
            glVertex2i(5+button_x_offset, button_y_offset + button_height-5);
          glEnd();
          glColor4f(1., 1., 1., 1.);
        }

        glBegin(GL_QUADS);
          glTexCoord2f(0.0, 0.0);
          glVertex2i(button_x_offset, button_y_offset);

          glTexCoord2f(1.0, 0.0);
          glVertex2i(button_x_offset + button_width, button_y_offset);

          glTexCoord2f(1.0, 1.0);
          glVertex2i(button_x_offset + button_width, button_y_offset + button_height);

          glTexCoord2f(0.0, 1.0);
          glVertex2i(button_x_offset, button_y_offset + button_height);
        glEnd();

        // Render "Credits"
        button_y_offset -= button_y_spacing;
        glBindTexture(GL_TEXTURE_2D, text_credits);

        if (current_menu_ == 2)
        {
          glColor4f(1., 1., 1., 0.5);
          glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex2i(5+button_x_offset, button_y_offset-5);

            glTexCoord2f(1.0, 0.0);
            glVertex2i(5+button_x_offset + button_width, button_y_offset-5);

            glTexCoord2f(1.0, 1.0);
            glVertex2i(5+button_x_offset + button_width, button_y_offset + button_height-5);

            glTexCoord2f(0.0, 1.0);
            glVertex2i(5+button_x_offset, button_y_offset + button_height-5);
          glEnd();
          glColor4f(1., 1., 1., 1.);
        }

        glBegin(GL_QUADS);
          glTexCoord2f(0.0, 0.0);
          glVertex2i(button_x_offset, button_y_offset);

          glTexCoord2f(1.0, 0.0);
          glVertex2i(button_x_offset + button_width, button_y_offset);

          glTexCoord2f(1.0, 1.0);
          glVertex2i(button_x_offset + button_width, button_y_offset + button_height);

          glTexCoord2f(0.0, 1.0);
          glVertex2i(button_x_offset, button_y_offset + button_height);
        glEnd();

        // Render "Quit"
        button_y_offset -= button_y_spacing;
        glBindTexture(GL_TEXTURE_2D, text_quit);

        if (current_menu_ == 3)
        {
          glColor4f(1., 1., 1., 0.5);
          glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex2i(5+button_x_offset, button_y_offset-5);

            glTexCoord2f(1.0, 0.0);
            glVertex2i(5+button_x_offset + button_width, button_y_offset-5);

            glTexCoord2f(1.0, 1.0);
            glVertex2i(5+button_x_offset + button_width, button_y_offset + button_height-5);

            glTexCoord2f(0.0, 1.0);
            glVertex2i(5+button_x_offset, button_y_offset + button_height-5);
          glEnd();
          glColor4f(1., 1., 1., 1.);
        }

        glBegin(GL_QUADS);
          glTexCoord2f(0.0, 0.0);
          glVertex2i(button_x_offset, button_y_offset);

          glTexCoord2f(1.0, 0.0);
          glVertex2i(button_x_offset + button_width, button_y_offset);

          glTexCoord2f(1.0, 1.0);
          glVertex2i(button_x_offset + button_width, button_y_offset + button_height);

          glTexCoord2f(0.0, 1.0);
          glVertex2i(button_x_offset, button_y_offset + button_height);
        glEnd();
    }

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

int main(int /*argc*/, char** /*argv*/)
{
    swiped::Engine& engine = swiped::Engine::instance();

    engine.set_scene(new MenuScene(engine));

    engine.start();

    return EXIT_SUCCESS;
}
