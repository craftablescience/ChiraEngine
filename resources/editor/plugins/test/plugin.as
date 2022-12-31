/*
    Example editor plugin for ChiraEngine
*/

class Example : Entity {
    Example() {
        super("Test");
    }

    void renderContents() {
        ImGui::Text("Hello World!");
        ImGui::Button("Test Button");
    }
}