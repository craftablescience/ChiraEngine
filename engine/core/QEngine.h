#pragma once

#include <QObject>
#include <QOpenGLWindow>
#include <core/Engine.h>

#include <QOpenGLFunctions_4_1_Core>

class QEngine : public QOpenGLWindow
{
public:
    QEngine(QWidget* parent);
private:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
};
