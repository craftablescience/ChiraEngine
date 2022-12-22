#pragma once

#include <entity/logic/Script.h>
#include <script/AngelScriptHolder.h>

#include <string>
namespace chira {
    class EditorPlugin {
        public:
        EditorPlugin(const std::string& ID);
        ~EditorPlugin();
        std::string getID();
        void doPlugin();
        void setVisible(bool visible);
        
        private:
        std::string ID;
        AngelScriptHolder script;
    };

    class EditorTool {
    public:
        EditorTool(const std::string& ID);
        virtual void renderContents();

        [[nodiscard]] std::string getID() const;
        [[nodiscard]] bool isVisible() const;
        void setVisible(bool visible);

    protected:
        std::string ID;
        bool visible = true;
    };
} // namespace chira