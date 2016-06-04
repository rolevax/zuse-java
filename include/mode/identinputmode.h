#ifndef IDENTINPUTMODE_H
#define IDENTINPUTMODE_H

#include "mode.h"

class IdentInputMode : public Mode
{
public:
    IdentInputMode(EditableDoc &doc, bool clear);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    static bool isType(const std::string &id);
    static int isModifier(const std::string &id);
    static bool isPrimitiveType(const std::string &id);
    static bool isUpperCamel(const std::string &id);
    Mode *promotion();
    Mode *promoteToDeclVar();
    Mode *promoteByKeyword();
    void setModifier(int modId);

private:
    bool clear;
};

#endif // IDENTINPUTMODE_H
