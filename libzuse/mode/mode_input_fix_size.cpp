#include "mode_input_fix_size.h"
#include "mode_menu.h"
#include "mode_input_string.h"

#include "../core/doc_editable.h"

#include <cassert>

ModeInputFixSize::ModeInputFixSize(DocEditable &doc, const AstInternal &f,
                                   size_t offset)
    : Mode(doc)
    , mAst(f)
    , mStage(offset)
    , mMacro(doc)
{
    assert(mAst.isFixSize());
}

Mode::Result ModeInputFixSize::keyboard(Key key)
{
    if (&mDoc.getOuter() == &mAst
            && mDoc.getInner().isList()
            && mDoc.getInner().asList().size() == 0
            && (key == Key::F || key == Key::S || key == Key::SPACE)) {
        // implements "fix-size forgetting" logic
        if (key == Key::F) {
            mDoc.assart(mDoc.getInner().asList().typeAt(0));
            // the user will focus on the list and forget about the fix-size
            return { ResultType::DONE_POP, mDoc.createModifyMode(true) };
        } else if (key == Key::S) {
            mDoc.sibling(-1, true);
            return DONE_POP_NOPUSH;
        } else { // key == Key::SPACE
            return nextStage();
        }
    } else {
        Mode *mode = nullptr;
        mMacro.macro(key, mode);
        return { ResultType::DONE_STAY, mode };
    }
}

Mode::Result ModeInputFixSize::onPushed()
{
    // assume all fix-size node has at least size one
    mDoc.fallIn();
    mDoc.sibling(int(mStage), false); // not skipping hidden node
    return pushOrWait();
}

Mode::Result ModeInputFixSize::onResume()
{
    return nextStage();
}

const char *ModeInputFixSize::name()
{
    switch (mAst.getType()) {
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

Mode::Result ModeInputFixSize::nextStage()
{
    // search toward root for the target fix-size node
    int digCount = 1;
    const Ast *outer = &mDoc.getOuter();
    while (outer != nullptr && outer->getType() != Ast::Type::CLASS_LIST
           && outer != &mAst) {
        outer = &outer->getParent();
        digCount++;
    }

    if (outer != &mAst) // the target fix-size node has gone
        return { ResultType::DONE_POP, nullptr };

    if (++mStage == mAst.size()) { // already input last child
        // cannot use single dig-out since some mode involves
        // unsymetric focus-in/dolly-out behaviors
        while (digCount --> 0)
            mDoc.digOut();

        return { ResultType::DONE_POP, nullptr };
    } else { // advance
        digCount--; // dolly-out 'upto', not 'onto'
        while (digCount --> 0)
            mDoc.digOut();

        mDoc.sibling(+1, true); // skip hidden node

        return pushOrWait();
    }
}

Mode::Result ModeInputFixSize::pushOrWait()
{
    // wait for user's choice (f or space) if inner is an empty list
    if (mDoc.getInner().isList() && mDoc.getInner().asList().size() == 0)
        return { ResultType::DONE_STAY, nullptr };
    else
        return { ResultType::DONE_STAY, mDoc.createModifyMode(true) };
}
