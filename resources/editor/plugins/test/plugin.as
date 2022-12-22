/*
    Example editor plugin for ChiraEngine
*/

class Plugin {
    bool visible = true;

    void doPlugin() {
        if (visible) {
            if (ImGui_Begin("Example Plugin", visible)) {
                ImGui_Text("Hello World!");
                ImGui_Button("Test Button");
            }
            ImGui_End();
        }
    }

    void setVisible(bool v) {
        visible = v;
    }
}