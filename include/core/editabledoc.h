#ifndef EDITABLEDOC_H
#define EDITABLEDOC_H

#include "ast/boplistast.h"
#include "mode/mode.h"

#include <string>
#include <functional>

class EditableDoc
{
public:
    enum class HotLightLevel { OFF, POINT, AREA };

    virtual ~EditableDoc() = default;

    virtual const InternalAst &getOuter() const = 0;
    virtual const Ast &getInner() const = 0;

    virtual void fallIn() = 0;
    virtual void digOut() = 0;
    virtual void sibling(int step) = 0;
    virtual void jackKick(bool down) = 0;
    virtual void hackLead(bool right) = 0;
    virtual void flyIn(std::function<bool(const Ast*)> &match) = 0;
    virtual void insert(Ast::Type type, int bop = BopListAst::UNUSED) = 0;
    virtual void append(Ast::Type type, int bop = BopListAst::UNUSED) = 0;
    virtual void assart(Ast::Type type, int bop = BopListAst::UNUSED) = 0;
    virtual void remove() = 0;
    virtual void change(Ast::Type type) = 0;
    virtual void nestAsLeft(Ast::Type type, int bop = BopListAst::UNUSED) = 0;
    virtual void nestAsRight(Ast::Type type, int bop = BopListAst::UNUSED) = 0;
    virtual void expose() = 0;
    virtual void cast(Ast::Type type) = 0;

    virtual Mode *createModifyMode(bool clear, size_t offset = 0) = 0;
    virtual void scalarAppend(const char *str) = 0;
    virtual void scalarAppend(char c) = 0;
    virtual void scalarClear() = 0;
    virtual void listClear() = 0;

    virtual void toggleAbstract() = 0;
    virtual void toggleFinal() = 0;
    virtual void toggleAccess() = 0;
    virtual void toggleStatic() = 0;
    virtual void toggleTransient() = 0;
    virtual void toggleVolatile() = 0;
    virtual void toggleNative() = 0;
    virtual void toggleSynchronized() = 0;

    virtual void setHotLight(HotLightLevel level) = 0;
    virtual void toggleTension(bool b) = 0;
};

#endif // EDITABLEDOC_H
