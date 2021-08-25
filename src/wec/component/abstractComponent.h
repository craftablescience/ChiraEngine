#pragma once

/*
 * How should you know whether to make something part of a
 * component or an entity? Simple. If it needs to render()
 * or stop(), or if it needs to have helper methods, then
 * it should be a component. If it doesn't need those, make
 * it part of the entity.
 */
class abstractComponent {
public:
    virtual void render(double delta) = 0;
    virtual void stop() = 0;
};
