#pragma once

#include <entity/logic/Script.h>
#include <script/AngelScriptHolder.h>

#include <string>
namespace chira {
    class EditorPlugin {
        public:
        EditorPlugin(std::string ID);
        std::string getID();
        void doPlugin();
        void setVisible(bool visible);
        
        private:
        std::string ID;
        AngelScriptHolder script;
    };
} // namespace chira