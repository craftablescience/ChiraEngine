#ifndef BASICGAMEENGINE_SHADERFILE_H
#define BASICGAMEENGINE_SHADERFILE_H


#include <string>
#include "../utility/handleObject.h"

class shaderFile : public handleObject {
public:
    shaderFile(unsigned int type, const std::string& source, bool isFilePath = true);
    virtual ~shaderFile();
    void compile() override;
    void discard() override;
    [[nodiscard]] unsigned int getType() const;
protected:
    unsigned int type;
    std::string source;
private:
    static std::string loadSourceFromFile(const std::string& filepath);
    void checkForCompilationErrors() const;
};


#endif //BASICGAMEENGINE_SHADERFILE_H
