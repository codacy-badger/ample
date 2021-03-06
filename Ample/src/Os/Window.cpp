#include <GL/gl.h>
#include <stdexcept>
#include <cassert>
#include <iostream>

#include "OsManager.h"
#include "Window.h"
#include "Error.h"

namespace os
{
Window::Window(const std::string &name,
               const size_t &x,
               const size_t &y,
               const size_t &width,
               const size_t &height,
               const os::winpos &posFlags,
               const os::winmode &modeFlags)
    : _winPtr(nullptr),
      _manager(new OsManager),
      _name(name),
      _x(x), _y(y),
      _width(width), _height(height),
      _modeFlags(static_cast<size_t>(modeFlags)),
      _glContext(nullptr),
      _contextX(0), _contextY(0),
      _contextW(width), _contextH(height)
{
    if (static_cast<size_t>(posFlags) & static_cast<uint32_t>(winpos::CENTERED_X))
    {
        _x = SDL_WINDOWPOS_CENTERED;
    }
    if (static_cast<size_t>(posFlags) & static_cast<uint32_t>(winpos::CENTERED_Y))
    {
        _y = SDL_WINDOWPOS_CENTERED;
    }
    if (static_cast<size_t>(posFlags) & static_cast<uint32_t>(winpos::UNDEFINED_POS))
    {
        _x = SDL_WINDOWPOS_UNDEFINED;
        _y = SDL_WINDOWPOS_UNDEFINED;
    }
}

Window::Window(const std::string &name,
               const size_t &x,
               const size_t &y,
               const size_t &width,
               const size_t &height,
               const os::winpos &posFlags,
               const os::winmode &modeFlags,
               const size_t &contextX,
               const size_t &contextY,
               const size_t &contextW,
               const size_t &contextH)
    : Window(name, x, y, width, height, posFlags, modeFlags)
{
    _contextX = contextX;
    _contextY = contextY;
    _contextW = contextW;
    _contextH = contextH;
}

void Window::setGlViewport(const size_t x, const size_t y, const size_t w, const size_t h)
{
    // bool xFit = (0 <= x + w && x + w <= _width);
    // bool yFit = (0 <= _contextY + _contextH && _contextY + _contextH <= _height);

    // if (!(xFit && yFit))
    // {
    //     throw exception::Exception(exception::exId::OPENGL_NOT_FIT,
    //                                exception::exType::CRITICAL);
    // }

    glViewport(x, y, w, h);
}

void Window::open()
{
    _winPtr = OsManager::createWindow(_name.c_str(),
                                            _x, _y,
                                            _width, _height,
                                            _modeFlags);

    _glContext = SDL_GL_CreateContext(_winPtr);

    setGlViewport(_contextX, _contextY, _contextW, _contextH);
}

void Window::swapBuffer()
{
    if (!_winPtr)
    {
        throw exception::Exception(exception::exId::WINDOW_NOT_READY,
                                   exception::exType::CRITICAL);
    }
    SDL_GL_SwapWindow(_winPtr);
}

size_t Window::getHeight()
{
    return _height;
}

size_t Window::getWidth()
{
    return _width;
}

void Window::resize(const size_t w, const size_t &h)
{
    _width = w;
    _height = h;
    setGlViewport(0, 0, _width, _height);
}

void Window::close()
{
    if (!_winPtr)
    {
        throw exception::Exception(exception::exId::WINDOW_NOT_READY,
                                   exception::exType::CRITICAL);
    }
    SDL_DestroyWindow(_winPtr);
    SDL_GL_DeleteContext(_glContext);
}

Window::~Window()
{
    delete _manager;
}
} // namespace os
