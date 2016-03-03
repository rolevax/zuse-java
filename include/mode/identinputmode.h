#ifndef IDENTINPUTMODE_H
#define IDENTINPUTMODE_H

#include "mode.h"

class IdentInputMode : public Mode
{
public:
    IdentInputMode(EditableDoc &doc, bool clear);

    void keyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    static bool isType(const std::string &id);
    static bool isPrimitiveType(const std::string &id);
    static bool isUpperCamel(const std::string &id);

private:
    bool clear;
};

#endif // IDENTINPUTMODE_H
