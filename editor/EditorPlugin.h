#pragma once

namespace chira {
    class EditorPlugin {
        public:
        EditorPlugin(std::string ID);
        std::string getID();

        protected:
        std::string ID;
    };
} // namespace chira