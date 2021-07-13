#ifndef BASICGAMEENGINE_ABSTRACTLOGGER_H
#define BASICGAMEENGINE_ABSTRACTLOGGER_H


enum loggerType {
    INFO,
    INFO_IMPORTANT,
    OUTPUT,
    WARNING,
    ERROR
};

class abstractLogger {
    virtual void log(loggerType type, const string& source, const string& message) = 0;
};


#endif //BASICGAMEENGINE_ABSTRACTLOGGER_H
