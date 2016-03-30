#include "core/editabledoc.h"
#include "mode/fixsizeinputmode.h"
#include "mode/menumode.h"
#include "mode/stringinputmode.h"

#include <cassert>

FixSizeInputMode::FixSizeInputMode(EditableDoc &doc, const InternalAst &f,
                                   size_t offset)
    : Mode(doc)
    , ast(f)
    , stage(offset)
    , macro(doc)
{
    assert(ast.isFixSize());
}

Mode::Result FixSizeInputMode::keyboard(Key key)
{
    Mode *mode = nullptr;
    macro.macro(key, mode);
    return { ResultType::DONE_STAY, mode };
}

Mode::Result FixSizeInputMode::onPushed()
{
    // assume all fix-size node has at least size one
    doc.fallIn();
    return { ResultType::DONE_STAY, doc.createModifyMode(true) };
}

Mode::Result FixSizeInputMode::onResume()
{
    if (++stage == ast.size()) {
        doc.digOut();
        return { ResultType::DONE_POP, nullptr };
    } else {
        doc.sibling(+1);
        return { ResultType::DONE_STAY, doc.createModifyMode(true) };
    }
}

const char *FixSizeInputMode::name()
{
    switch (ast.getType()) {
    case Ast::Type::DECL_CLASS:
        return "Class";
    case Ast::Type::DECL_METHOD:
        return "Method";
    case Ast::Type::DECL_PARAM:
        return "Param";
    case Ast::Type::DECL_VAR:
        return "Var";
    default:
        return "Fix";
    }
}
