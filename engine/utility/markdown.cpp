#include "markdown.h"

using namespace chira;

std::unique_ptr<ImGui::MarkdownConfig> Markdown::mdConfig = std::make_unique<ImGui::MarkdownConfig>(); // NOLINT(cert-err58-cpp)
