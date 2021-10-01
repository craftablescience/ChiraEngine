#pragma once

/*
 * Q: When do I make something part of a component or an entity?
 * A: If it should use functionality from other components, and/or
 *    stand alone in the world, make it an entity.
 */
class abstractComponent {
public:
    virtual void render(double delta) = 0;
    virtual void stop() = 0;
};
