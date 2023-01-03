/*
 * QT SDL Window Wrapper
 *
 * TODO: Make a proper Qt backend
 *
 * This works fine for the most part but it's a bit of a hack.
 *
 * This should not be considered the same as proper Qt backend support
 * as it was implemented specifically as a proof of concept to show a Qt
 * GUI integrated with ChiraEngine for engine tools in another project.
 *
 * The implementation was done to allow for the Engine to be compiled with SDL
 * but allow for using a command line flag to open an editor built with Qt that
 * still integrates with the engine.
 *
 * Credit to JJl77 for the base code for this
 * Modified heavily to work with updated SDL and Chira by ashifolfi
 */

#include "qtwindow.h"
#include <core/Engine.h>
#include <core/Logger.h>

#include <private/qwindow_p.h>

using namespace ui;

CHIRA_CREATE_LOG(QTWINDOW);

//-----------------------------------------------------------------//
// Construct around existing window
//-----------------------------------------------------------------//
ui::Window::Window()
{
    m_pHwnd = chira::Engine::getDevice()->getWindow();
    // Create the window container and stuff
    CreateWindow();
}

//-----------------------------------------------------------------//
// Cleanup
//-----------------------------------------------------------------//
ui::Window::~Window()
{
}

//-----------------------------------------------------------------//
// Create the QWindow, hookup signals, etc.
//-----------------------------------------------------------------//
void ui::Window::CreateWindow()
{
    LOG_QTWINDOW.output("Embedding Chira SDL window as QWidget...");
    int w, h;
    SDL_GetWindowSize( m_pHwnd, &w, &h );
    m_nWidth = w;
    m_nHeight = h;

    /* This is the only tricky part and it's platform specific */
    SDL_SysWMinfo info {};
    // this is very important yes.
    SDL_VERSION(&info.version);
    if (!SDL_GetWindowWMInfo( m_pHwnd, &info )) {
        LOG_QTWINDOW.error(SDL_GetError());
    }

    // See: https://wiki.libsdl.org/SDL2/SDL_SysWMinfo
    WId nativeWindow;

    // yes this is absolutely neccessary to do because if we don't have the
    // driver then we don't have access to the member.
    switch( info.subsystem )
    {
#ifdef SDL_VIDEO_DRIVER_X11
    case SDL_SYSWM_X11:
        nativeWindow = (WId)info.info.x11.window;
        break;
#endif

#ifdef SDL_VIDEO_DRIVER_WAYLAND
    case SDL_SYSWM_WAYLAND: // no idea if this works i don't use wayland
        nativeWindow = (WId)info.info.wl.egl_window;
        break;
#endif

#ifdef SDL_VIDEO_DRIVER_WINDOWS
    case SDL_SYSWM_WINDOWS:
        nativeWindow = (WId)info.info.win.window;
        break;
#endif

#ifdef SDL_VIDEO_DRIVER_COCOA
    case SDL_SYSWM_COCOA:
        nativeWindow = (WId)info.info.cocoa.window;
        break;
#endif
    default:
        throw std::runtime_error("Unsupported Video Driver! Unable to embed!");
    }

    // for sanity checking!
    if (!nativeWindow)
        throw std::runtime_error("nativeWindow empty! embed failed!");

    // Accessing private data is generally bad, but it's the easiest way to work with this.
    // Only other way to embed a window is using a static QWindow method, which doesn't actually let us create the underlying surface.
    qt_window_private(this)->create(false, nativeWindow );

    // Don't want a frame on this, since it's meant to be embedded
    setFlag( Qt::FramelessWindowHint );

    // Create the actual window container widget, install the event handler
    m_pWidget = QWidget::createWindowContainer( this );
    m_pWidget->installEventFilter( this );
}

//-----------------------------------------------------------------//
// Resize the window before rendering. Sets mode and changes backbuffer
// size.
//-----------------------------------------------------------------//
void ui::Window::UpdateWindowSize()
{
    // Account for DPI scaling here using devicePixelRatio(), size values are all scaled by this.
    auto scr = m_pWidget->screen();
    this->Resize( width() * scr->devicePixelRatio(), height() * scr->devicePixelRatio() );
}

//-----------------------------------------------------------------//
// Helper to resize the window without changing flags
//-----------------------------------------------------------------//
void ui::Window::Resize( int w, int h )
{
    SDL_SetWindowSize( m_pHwnd, w, h );
    m_nHeight = h;
    m_nWidth = w;
}

//-----------------------------------------------------------------//
// Returns actual window width, according to the WM
//-----------------------------------------------------------------//
int ui::Window::GetWidth() const
{
    int w, h;
    GetSize( w, h );
    return w;
}

//-----------------------------------------------------------------//
// Returns actual window height, according to the WM
//-----------------------------------------------------------------//
int ui::Window::GetHeight() const
{
    int w, h;
    GetSize( w, h );
    return h;
}

void ui::Window::GetSize( int& w, int& h ) const
{
    SDL_GetWindowSize( m_pHwnd, &w, &h );
}

//-----------------------------------------------------------------//
// Show the window when Qt wants us to
//-----------------------------------------------------------------//
void ui::Window::showEvent( QShowEvent* ev )
{
    SDL_ShowWindow( m_pHwnd );
    QWindow::showEvent( ev );
}

//-----------------------------------------------------------------//
// Hide the window when Qt wants us to
//-----------------------------------------------------------------//
void ui::Window::hideEvent( QHideEvent* ev )
{
    SDL_HideWindow( m_pHwnd );
    QWindow::hideEvent( ev );
}
