#include "resourcebrowser.h"
#include "ui_resourcebrowser.h"

#include <i18n/TranslationManager.h>

ResourceBrowser::ResourceBrowser(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ResourceBrowser)
{
    ui->setupUi(this);
    setWindowTitle(TR("ui.resourcebrowser.title").c_str());
}

ResourceBrowser::~ResourceBrowser()
{
    delete ui;
}
