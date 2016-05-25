#include "mode/identinputmode.h"
#include "core/editabledoc.h"
#include "ast/scalarast.h"

#include <algorithm>
#include <cstdlib>
#include <cassert>

IdentInputMode::IdentInputMode(EditableDoc &doc, bool clear)
    : Mode(doc),
      clear(clear)
{

}

Mode::Result IdentInputMode::keyboard(Key key)
{
    assert(doc.getInner().getType() == Ast::Type::IDENT);

    if (key == Key::SPACE) {
        Mode *nextPush = promotion();
        return { ResultType::DONE_POP, nextPush };
    }

    if (clear) {
        doc.setHotLight(EditableDoc::HotLightLevel::POINT);
        doc.scalarClear();
        clear = false;
    }

    if (KeyCode::isAlpha(key) || KeyCode::isDigit(key)
            || key == Key::BACKSPACE || key == Key::UNDERSCORE
            || key == Key::DOLLAR) {
        doc.scalarAppend(KeyCode::toChar(key));
        return DONE_STAY_NOPUSH;
    } else {
        return RAISE_POP_NOPUSH;
    }
}

Mode::Result IdentInputMode::onPushed()
{
    if (clear)
        doc.setHotLight(EditableDoc::HotLightLevel::AREA);
    else
        doc.setHotLight(EditableDoc::HotLightLevel::POINT);
    return DONE_STAY_NOPUSH;
}

void IdentInputMode::onPopped()
{
    doc.setHotLight(EditableDoc::HotLightLevel::OFF);
}

const char *IdentInputMode::name()
{
    return "Ident Input";
}

bool IdentInputMode::isType(const std::string &id)
{
    return isPrimitiveType(id) || isUpperCamel(id);
}

int IdentInputMode::isModifier(const std::string &id)
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

bool IdentInputMode::isPrimitiveType(const std::string &id)
{
    return id == "void" || id == "byte" || id == "short"
            || id == "int" || id == "long" || id == "char"
            || id == "float" || id == "double";
}

bool IdentInputMode::isUpperCamel(const std::string &id)
{
    // at least size 2, initial is upper, at least one lower
    return id.size() > 1 && isupper(id[0])
            && id.end() != std::find_if(id.begin() + 1, id.end(), islower);
}

Mode *IdentInputMode::promotion()
{
    if (Mode *ret = promoteToDeclVar())
        return ret;

    if (Mode *ret = promoteToStmt())
        return ret;

    return nullptr;
}

Mode *IdentInputMode::promoteToDeclVar()
{
    auto canHaveDeclVar = [](Ast::Type ot, size_t inner)
    {
        return ot == Ast::Type::STMT_LIST
                || ot == Ast::Type::MEMBER_LIST
                || (ot == Ast::Type::FOR && inner == 0);
    };

    const std::string &text = doc.getInner().asScalar().getText();
    if (isType(text)) {
        // case 1: check bare identifier
        Ast::Type otype = doc.getOuter().getType();
        if (canHaveDeclVar(otype, doc.getInnerIndex())) {
            doc.nestAsLeft(Ast::Type::DECL_VAR);
            return doc.createModifyMode(true, 1);
        }

        // case 2: check qualified name
        if (otype == Ast::Type::DOT_BOP_LIST) {
            const BopListAst &bast = doc.getOuter().asBopList();
            assert(bast.size() >= 2);
            size_t i = 1;
            while (i != bast.size() && bast.opAt(i) == BopListAst::DOT)
                i++;
            if (i == bast.size()) { // all op is member access
                Ast::Type ootype = doc.getOuter().getParent().getType();
                size_t oInOo = doc.getOuter().getParent().indexOf(&doc.getOuter());
                if (canHaveDeclVar(ootype, oInOo)) {
                    doc.digOut();
                    doc.nestAsLeft(Ast::Type::DECL_VAR);
                    return doc.createModifyMode(true, 1);
                }
            }
        }
    } else if (int modId = isModifier(text)) {
        Ast::Type otype = doc.getOuter().getType();
        if (canHaveDeclVar(otype, doc.getInnerIndex())) {
            doc.change(Ast::Type::DECL_VAR);
            setModifier(modId);
            return doc.createModifyMode(true, 0);
        } else if (otype == Ast::Type::DECL_PARAM) {
            doc.digOut();
            doc.toggleFinal();
            doc.fallIn();
            // silent onResume() if fix-size-mode and re-input the identifier
            return doc.createModifyMode(true, 0);
        }
    }

    return nullptr;
}

Mode *IdentInputMode::promoteToStmt()
{
    const std::string &text = doc.getInner().asScalar().getText();
    if (text == "while") {
        doc.change(Ast::Type::WHILE);
        return doc.createModifyMode(true, 0);
    } else if (text == "if") {
        doc.change(Ast::Type::IF_CONDBODY);
        return doc.createModifyMode(true, 0);
    } else if (text == "do") {
        doc.change(Ast::Type::DO_WHILE);
        return doc.createModifyMode(true, 0);
    } else if (text == "for") {
        doc.change(Ast::Type::FOR);
        return doc.createModifyMode(true, 0);
    } else if (text == "return") {
        doc.change(Ast::Type::RETURN);
        return doc.createModifyMode(true, 0);
    } else if (text == "try") {
        doc.change(Ast::Type::TRY_LIST);
        doc.fallIn(); // now inner is stmt_list
        return doc.createModifyMode(true); // a list input mode
    }

    return nullptr;
}

void IdentInputMode::setModifier(int modId)
{
    if (modId == 1)
        doc.toggleAbstract();
    else if(modId == 2)
        doc.toggleFinal();
    else if (modId == 3)
        doc.toggleAccess(true);
    else if (modId == 4)
        doc.toggleAccess(true), doc.toggleAccess(true);
    else if (modId == 5)
        doc.toggleAccess(false);
    else if (modId == 6)
        doc.toggleStatic();
    else if (modId == 7)
        doc.toggleTransient();
    else if (modId == 8)
        doc.toggleVolatile();
    else if (modId == 9)
        doc.toggleNative();
    else if (modId == 10)
        doc.toggleSynchronized();
}

