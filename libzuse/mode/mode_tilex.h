#ifndef ZUSE_MODE_TILEX_H
#define ZUSE_MODE_TILEX_H

#include "mode.h"



namespace zuse
{



///
/// \brief Tiny-lexer mode
///
class ModeTilex : public Mode
{
public:
    ModeTilex(DocEditable &mDoc, bool mMacroContext);

    Result keyboard(Key key) override;
    Result onPushed() override;
    const char *name() override;

private:
    Result keyboardSpace();
    Result keyboardEqual();
    Result ppmm(bool inc);
    void castOuter(Ast::Type to);
    void relayMacro(int savedInner);

private:
    bool mMacroContext;
};



} // namespace zuse



#endif // ZUSE_MODE_TILEX_H
