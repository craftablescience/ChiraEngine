/*
    Example editor plugin for ChiraEngine
*/

class ExamplePlugin : EditorPlugin {
    ExamplePlugin() {
        super("ExamplePlugin");
    }
    
    void doPlugin() {
        print("Hello World!")
        // ImGui.SetNextWindowSize(ImVec2(200,200), ImGuiCond_Always);
        // if (ImGui.Begin("Example Plugin")) {
        //     ImGui.Text("Hello World!");
        // }
        // ImGui.End();
    }
}

void update() {
    print("Hello World!");
}