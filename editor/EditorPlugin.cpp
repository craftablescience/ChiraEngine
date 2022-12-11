#include "EditorPlugin.h"

using namespace chira;

EditorPlugin::EditorPlugin(std::string ID) {
    this->ID = ID;
}

std::string EditorPlugin::getID() {
    return this->ID;
}

void EditorPlugin::doPlugin() {

}