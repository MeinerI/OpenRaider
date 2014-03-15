/*!
 * \file include/WindowSDL.h
 * \brief SDL windowing implementation
 *
 * \author xythobuz
 */

#ifndef _WINDOW_SDL_H_
#define _WINDOW_SDL_H_

#include "SDL.h"
#include "SDL_ttf.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Window.h"

/*!
 * \brief SDL windowing implementation
 */
class WindowSDL : public Window {
public:

    /*!
     * \brief Constructs an object of WindowSDL
     */
    WindowSDL();

    /*!
     * \brief Deconstructs an object of WindowSDL
     */
    virtual ~WindowSDL();

    virtual void setDriver(const char *driver);

    virtual void setSize(unsigned int width, unsigned int height);

    virtual void setFullscreen(bool fullscreen);

    virtual void setMousegrab(bool grab);

    virtual int initialize();

    virtual void eventHandling();

    virtual void delay(clock_t ms);

    virtual void swapBuffersGL();

    virtual void setFont(const char *font);

    virtual int initializeFont();

    virtual void writeString(WindowString *s);

private:
    bool mInit;
    char *mDriver;
    unsigned int mWidth;
    unsigned int mHeight;
    bool mFullscreen;
    bool mMousegrab;
    SDL_Window *mWindow;      //!< This is the pointer to the SDL surface
    SDL_GLContext mGLContext; //!< The OpenGL Context

    bool mFontInit;
    char *mFontName;
    TTF_Font *mFont;
    GLuint mFontTexture;
};

#endif