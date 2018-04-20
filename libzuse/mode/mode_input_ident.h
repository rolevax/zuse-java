#ifndef ZUSE_MODE_INPUT_IDENT_H
#define ZUSE_MODE_INPUT_IDENT_H

#include "mode.h"

class ModeInputIdent : public Mode
{
public:
    ModeInputIdent(DocEditable &mDoc, bool mClear);

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
    bool mClear;
};

#endif // ZUSE_MODE_INPUT_IDENT_H
