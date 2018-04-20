#include "mode_input_ident.h"

#include "../core/doc_editable.h"
#include "../ast/ast_scalar.h"

#include <algorithm>
#include <cstdlib>
#include <cassert>

ModeInputIdent::ModeInputIdent(DocEditable &doc, bool clear)
    : Mode(doc),
      mClear(clear)
{

}

Mode::Result ModeInputIdent::keyboard(Key key)
{
    assert(mDoc.getInner().getType() == Ast::Type::IDENT);

    if (key == Key::SPACE) {
        Mode *nextPush = promotion();
        return { ResultType::DONE_POP, nextPush };
    }

    if (mClear) {
        mDoc.setHotLight(DocEditable::HotLightLevel::POINT);
        mDoc.scalarClear();
        mClear = false;
    }

    if (KeyCode::isAlpha(key) || KeyCode::isDigit(key)
            || key == Key::BACKSPACE || key == Key::UNDERSCORE
            || key == Key::DOLLAR) {
        mDoc.scalarAppend(KeyCode::toChar(key));
        return DONE_STAY_NOPUSH;
    } else {
        return RAISE_POP_NOPUSH;
    }
}

Mode::Result ModeInputIdent::onPushed()
{
    if (mClear)
        mDoc.setHotLight(DocEditable::HotLightLevel::AREA);
    else
        mDoc.setHotLight(DocEditable::HotLightLevel::POINT);
    return DONE_STAY_NOPUSH;
}

void ModeInputIdent::onPopped()
{
    mDoc.setHotLight(DocEditable::HotLightLevel::OFF);
}

const char *ModeInputIdent::name()
{
    return "Ident Input";
}

bool ModeInputIdent::isType(const std::string &id)
{
    return isPrimitiveType(id) || isUpperCamel(id);
}

int ModeInputIdent::isModifier(const std::string &id)
{
    if (id == "abstract") return 1;
    if (id == "final") return 2;
    if (id == "public") return 3;
    if (id == "protected") return 4;
    if (id == "private") return 5;
    if (id == "static") return 6;
    if (id == "transient") return 7;
    if (id == "volatile") return 8;
    if (id == "native") return 9;
    if (id == "synchronized") return 10;
    return 0;
}

bool ModeInputIdent::isPrimitiveType(const std::string &id)
{
    return id == "void" || id == "byte" || id == "short"
            || id == "int" || id == "long" || id == "char"
            || id == "float" || id == "double" || id == "boolean";
}

bool ModeInputIdent::isUpperCamel(const std::string &id)
{
    // at least size 2, initial is upper, at least one lower
    return id.size() > 1 && isupper(id[0])
            && id.end() != std::find_if(id.begin() + 1, id.end(), islower);
}

Mode *ModeInputIdent::promotion()
{
    if (Mode *ret = promoteToDeclVar())
        return ret;

    if (Mode *ret = promoteByKeyword())
        return ret;

    return nullptr;
}

Mode *ModeInputIdent::promoteToDeclVar()
{
    auto canHaveDeclVar = [](Ast::Type ot, size_t inner)
    {
        return ot == Ast::Type::STMT_LIST
                || ot == Ast::Type::MEMBER_LIST
                || (ot == Ast::Type::FOR && inner == 0);
    };

    const std::string &text = mDoc.getInner().asScalar().getText();
    if (isType(text)) {
        // case 1: check bare identifier
        Ast::Type otype = mDoc.getOuter().getType();
        if (canHaveDeclVar(otype, mDoc.getInnerIndex())) {
            mDoc.nestAsLeft(Ast::Type::DECL_VAR);
            return mDoc.createModifyMode(true, 1);
        }

        // case 2: check qualified name
        if (otype == Ast::Type::DOT_BOP_LIST) {
            const AstListBop &bast = mDoc.getOuter().asBopList();
            assert(bast.size() >= 2);
            size_t i = 1;
            while (i != bast.size() && bast.opAt(i) == AstListBop::DOT)
                i++;
            if (i == bast.size()) { // all op is member access
                Ast::Type ootype = mDoc.getOuter().getParent().getType();
                size_t oInOo = mDoc.getOuter().getParent().indexOf(&mDoc.getOuter());
                if (canHaveDeclVar(ootype, oInOo)) {
                    mDoc.digOut();
                    mDoc.nestAsLeft(Ast::Type::DECL_VAR);
                    return mDoc.createModifyMode(true, 1);
                }
            }
        }
    } else if (int modId = isModifier(text)) {
        Ast::Type otype = mDoc.getOuter().getType();
        if (canHaveDeclVar(otype, mDoc.getInnerIndex())) {
            mDoc.change(Ast::Type::DECL_VAR);
            setModifier(modId);
            return mDoc.createModifyMode(true, 0);
        } else if (otype == Ast::Type::DECL_PARAM) {
            mDoc.digOut();
            mDoc.toggleFinal();
            mDoc.fallIn();
            // silent onResume() if fix-size-mode and re-input the identifier
            return mDoc.createModifyMode(true, 0);
        }
    }

    return nullptr;
}

Mode *ModeInputIdent::promoteByKeyword()
{
    const std::string &text = mDoc.getInner().asScalar().getText();
    if (text == "while") {
        mDoc.change(Ast::Type::WHILE);
        return mDoc.createModifyMode(true, 0);
    } else if (text == "if") {
        mDoc.change(Ast::Type::IF_CONDBODY);
        return mDoc.createModifyMode(true, 0);
    } else if (text == "do") {
        mDoc.change(Ast::Type::DO_WHILE);
        return mDoc.createModifyMode(true, 0);
    } else if (text == "for") {
        mDoc.change(Ast::Type::FOR);
        return mDoc.createModifyMode(true, 0);
    } else if (text == "return") {
        mDoc.change(Ast::Type::RETURN);
        return nullptr; // manually punch or not punch
    } else if (text == "try") {
        mDoc.change(Ast::Type::TRY_LIST);
        mDoc.fallIn(); // now inner is stmt_list
        return mDoc.createModifyMode(true); // a list input mode
    } else if (text == "new") {
        mDoc.change(Ast::Type::NEW_CLASS);
        return mDoc.createModifyMode(true, 0);
    }

    return nullptr;
}

void ModeInputIdent::setModifier(int modId)
{
    if (modId == 1)
        mDoc.toggleAbstract();
    else if(modId == 2)
        mDoc.toggleFinal();
    else if (modId == 3)
        mDoc.toggleAccess(true);
    else if (modId == 4)
        mDoc.toggleAccess(true), mDoc.toggleAccess(true);
    else if (modId == 5)
        mDoc.toggleAccess(false);
    else if (modId == 6)
        mDoc.toggleStatic();
    else if (modId == 7)
        mDoc.toggleTransient();
    else if (modId == 8)
        mDoc.toggleVolatile();
    else if (modId == 9)
        mDoc.toggleNative();
    else if (modId == 10)
        mDoc.toggleSynchronized();
}

