#include "EditorPlugin.h"

using namespace chira;

EditorPlugin::EditorPlugin(std::string ID) : script("file://plugins/"+ID+"/plugin.as") {
    this->ID = ID;
    this->doPlugin = this->script.getFunction("doPlugin");
}

std::string EditorPlugin::getID() {
    return this->ID;
}