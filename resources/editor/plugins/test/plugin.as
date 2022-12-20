/*
    Example editor plugin for ChiraEngine
*/

bool p_visible = true;

void doPlugin() {
    if (p_visible) {
        if (ImGui_Begin("Example Plugin", p_visible)) {
            ImGui_Text("Hello World!");
            ImGui_Button("Test Button");
        }
        ImGui_End();
    }
}

void setVisible() {
    p_visible = true;
}