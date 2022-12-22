#include "EditorPlugin.h"

using namespace chira;

EditorPlugin::EditorPlugin(std::string ID) : script("file://plugins/"+ID+"/plugin.as") {
    this->ID = ID;
}

void EditorPlugin::setVisible(bool visible) {
    this->script.callFunction<void>("setVisible", visible);
}

void EditorPlugin::doPlugin() {
    this->script.callFunction<void>("doPlugin");
}

std::string EditorPlugin::getID() {
    return this->ID;
}

EditorPlugin::~EditorPlugin() {
    this->script.callFunction<void>("stop");
}