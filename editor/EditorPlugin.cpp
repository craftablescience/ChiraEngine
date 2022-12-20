#include "EditorPlugin.h"

using namespace chira;

EditorPlugin::EditorPlugin(std::string ID) : script("file://plugins/"+ID+"/plugin.as") {
    this->ID = ID;
    this->doPlugin = this->script.getFunction("doPlugin");
}

void EditorPlugin::setVisible(bool visible) {
    this->script.getFunction("setVisible")();
}

std::string EditorPlugin::getID() {
    return this->ID;
}