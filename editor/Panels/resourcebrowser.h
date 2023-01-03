#pragma once

#include <QDockWidget>

namespace Ui {
class ResourceBrowser;
}

class ResourceBrowser : public QDockWidget
{
    Q_OBJECT

public:
    explicit ResourceBrowser(QWidget *parent = nullptr);
    ~ResourceBrowser();

private:
    Ui::ResourceBrowser *ui;
};
