#pragma once

namespace chira {
    class EditorPlugin {
        public:
        EditorPlugin(std::string ID);
        std::string getID();

        virtual void doPlugin();

        protected:
        std::string ID;
    };
} // namespace chira