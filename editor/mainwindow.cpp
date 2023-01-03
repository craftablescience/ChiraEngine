// class includes
#include "mainwindow.h"
#include "ui_mainwindow.h"

// chira engine includes
#include <core/Engine.h>
#include <i18n/TranslationManager.h>
#include <ui/qt/qtwindow.h>

// qt includes
#include <QLayout>

using namespace chira;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto* window = new ui::Window();
    this->layout()->addWidget(window->GetWidget());
    ui->dockWidget->setWindowTitle(TR("ui.resbrowser.title").c_str());
    this->setWindowTitle(TR("ui.qtwindow.title").c_str());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent (QCloseEvent *event) {
    Engine::getDevice()->closeAfterThisFrame();
}
