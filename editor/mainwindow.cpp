#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ui/qt/qtwindow.h>
#include <QLayout>

#include "Editor.h"
#include <core/Engine.h>
#include <i18n/TranslationManager.h>

using namespace chira;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto* window = new ui::Window();
    layout()->addWidget(window->GetWidget());
    this->setWindowTitle(TR("ui.qtwindow.title").c_str());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ceShowGrid_stateChanged(int arg1)
{
    ModelViewerPanel *editpanel = (ModelViewerPanel *)Engine::getDevice()->getPanel(mainpanelid);
    editpanel->showGrid = (bool)arg1;
}

