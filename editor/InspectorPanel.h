#pragma once

#include <ui/IPanel.h>
#include <entity/Entity.h>

namespace chira {
	class InspectorPanel : public IPanel {
		public:
			InspectorPanel();

			void renderContents() override;

			void setEntity(Entity *newEnt);
			Entity *getEntity();

		private:
			Entity *curEnt;
	};
} // namespace chira