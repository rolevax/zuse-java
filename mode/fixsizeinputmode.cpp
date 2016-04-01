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
    if (&doc.getOuter() == &ast
            && doc.getInner().isList()
            && doc.getInner().asList().size() == 0
            && (key == Key::F || key == Key::SPACE)) {
        // implements "fix-size forgetting" logic
        if (key == Key::F) {
            doc.assart(doc.getInner().asList().typeAt(0));
            // the user will focus on the list and forget about the fix-size
            return { ResultType::DONE_POP, doc.createModifyMode(true) };
        } else { // key == Key::SPACE
            return nextStage();
        }
    } else {
        Mode *mode = nullptr;
        macro.macro(key, mode);
        return { ResultType::DONE_STAY, mode };
    }
}

Mode::Result FixSizeInputMode::onPushed()
{
    // assume all fix-size node has at least size one
    doc.fallIn();
    doc.sibling(int(stage));
    return pushOrWait();
}

Mode::Result FixSizeInputMode::onResume()
{
    return nextStage();
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

Mode::Result FixSizeInputMode::nextStage()
{
    // search toward root for the target fix-size node
    int digCount = 1;
    const Ast *outer = &doc.getOuter();
    while (outer != nullptr && outer->getType() != Ast::Type::CLASS_LIST
           && outer != &ast) {
        outer = &outer->getParent();
        digCount++;
    }

    if (outer != &ast) // the target fix-size node has gone
        return { ResultType::DONE_POP, nullptr };

    if (++stage == ast.size()) { // already input last child
        // cannot use single dig-out since some mode involves
        // unsymetric fall-dig behaviors
        while (digCount --> 0)
            doc.digOut();

        return { ResultType::DONE_POP, nullptr };
    } else { // advance
        digCount--; // dig-out 'upto', not 'onto'
        while (digCount --> 0)
            doc.digOut();

        doc.sibling(+1);

        return pushOrWait();
    }
}

Mode::Result FixSizeInputMode::pushOrWait()
{
    // wait for user's choice (f or space) if inner is an empty list
    if (doc.getInner().isList() && doc.getInner().asList().size() == 0)
        return { ResultType::DONE_STAY, nullptr };
    else
        return { ResultType::DONE_STAY, doc.createModifyMode(true) };
}
