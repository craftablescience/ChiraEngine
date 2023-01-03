#pragma once

#include <QWidget>
#include <QWindow>
#include <QScreen>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

struct SDL_Window;

namespace ui {

class Window : public QWindow
{
    Q_OBJECT;
public:
    Window();

    ~Window();

    //--------------------------------------------------------------------------------//
    // Returns the embed widget for this window
    // This is also the event target where this windows' events will be forwarded to
    //--------------------------------------------------------------------------------//
    QWidget* GetWidget() const { return m_pWidget; }

    //--------------------------------------------------------------------------------//
    // Returns the actual size of the window, as reported by the WM/Compositor
    // This is not necessarily the size of the backbuffer
    //--------------------------------------------------------------------------------//
    int GetWidth() const;
    int GetHeight() const;
    void GetSize( int& w, int& h ) const;

    //--------------------------------------------------------------------------------//
    // Should be called before rendering to this window!
    // Resizes the internal representation of the window (incl. backbuffer)
    // to fit the "actual" size.
     //--------------------------------------------------------------------------------//
    void UpdateWindowSize();

protected:
    void showEvent( QShowEvent* ev ) override;
    void hideEvent( QHideEvent* ev ) override;

private:
    SDL_Window* m_pHwnd = nullptr;
    QWidget* m_pWidget = nullptr;
    int m_nWidth = 0;
    int m_nHeight = 0;

    void CreateWindow();
    void Resize( int w, int h );
};

}
