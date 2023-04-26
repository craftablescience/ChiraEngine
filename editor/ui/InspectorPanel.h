#pragma once

#include <entity/Entity.h>
#include <ui/IPanel.h>
#include <imfilebrowser.h>

#include <entity/component/AngelScriptComponent.h>
#include <entity/component/AudioNoiseComponent.h>
#include <entity/component/AudioSfxrComponent.h>
#include <entity/component/AudioSpeechComponent.h>
#include <entity/component/AudioWavComponent.h>
#include <entity/component/AudioWavStreamComponent.h>
#include <entity/component/CameraComponent.h>
#include <entity/component/LightComponents.h>
#include <entity/component/MeshComponent.h>
#include <entity/component/MeshDynamicComponent.h>
#include <entity/component/NameComponent.h>
#include <entity/component/SkyboxComponent.h>
#include <entity/component/TagComponents.h>
#include <entity/component/TransformComponent.h>
#include <entity/component/UUIDComponent.h>

namespace chira {

class InspectorPanel : public IPanel {
public:
    InspectorPanel();

    void renderContents() override;

    void setEntity(Entity* newEnt);
    [[nodiscard]] Entity* getEntity() const;

private:
    Entity* curEnt;
};

} // namespace chira
