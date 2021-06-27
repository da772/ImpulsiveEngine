#pragma once

#include "EditorModule.h"


namespace Editor {

    class DockModule : public EditorModule
    {
    public:
        DockModule(const std::vector< std::pair < std::string, std::string>>& nodes);
        ~DockModule();
        void Create(const std::string& name, bool* is_open, uint32_t flags) override;

    private:
        std::vector<std::pair<std::string, std::string>> m_nodes;
    };

}

