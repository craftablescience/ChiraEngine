#ifndef BASICGAMEENGINE_ABSTRACTSCRIPTPROVIDER_H
#define BASICGAMEENGINE_ABSTRACTSCRIPTPROVIDER_H


class abstractScriptProvider {
public:
    virtual void initProvider() = 0;
    virtual void initScripts() = 0;
    virtual void render(double delta) = 0;
    virtual void stop() = 0;
};


#endif //BASICGAMEENGINE_ABSTRACTSCRIPTPROVIDER_H
