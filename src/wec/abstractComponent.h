#pragma once

#include "../resource/propertiesResource.h"
#include "json/json.h"

namespace chira {
    /// Q: When do I make something part of a component or an entity? \n
    /// A: If it should use functionality from other components and stand alone
    ///    in the world, make it an entity. If it provides self-contained
    ///    functionality, and should be a part of something greater than itself,
    ///    make it a component.
    class abstractComponent {
    public:
        virtual void render(double delta) = 0;
        virtual ~abstractComponent() = default;
    };
}
