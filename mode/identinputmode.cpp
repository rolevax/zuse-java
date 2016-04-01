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
    if (isType(doc.getInner().asScalar().getText())) {
        // case 1: check bare identifier
        Ast::Type otype = doc.getOuter().getType();
        if (otype == Ast::Type::STMT_LIST || otype == Ast::Type::MEMBER_LIST) {
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
                if (ootype == Ast::Type::STMT_LIST
                        || ootype == Ast::Type::MEMBER_LIST) {
                    doc.digOut();
                    doc.nestAsLeft(Ast::Type::DECL_VAR);
                    return doc.createModifyMode(true, 1);
                }
            }
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
        doc.change(Ast::Type::IF_LIST);
        return doc.createModifyMode(true, 0);
    } else if (text == "do") {
        doc.change(Ast::Type::DO_WHILE);
        return doc.createModifyMode(true, 0);
    } else if (text == "return") {
        doc.change(Ast::Type::RETURN);
        return doc.createModifyMode(true, 0);
    }

    return nullptr;
}

