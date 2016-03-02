#include "mode/identinputmode.h"
#include "core/editabledoc.h"
#include "ast/scalarast.h"

#include <algorithm>
#include <cassert>

IdentInputMode::IdentInputMode(EditableDoc &doc, bool clear)
    : Mode(doc),
      clear(clear)
{

}

void IdentInputMode::keyboard(char key)
{
    if (key == ' ') {
        doc.pop();
    } else {
        // check legal char
        doc.scalarAppend(key);
    }
}

void IdentInputMode::onPushed()
{
    if (clear) {
        assert(doc.getInner().getType() == Ast::Type::IDENT);
        doc.scalarClear();
    }

    doc.setHotLight(EditableDoc::HotLightLevel::POINT);
}

void IdentInputMode::onPopped()
{
    doc.setHotLight(EditableDoc::HotLightLevel::OFF);

    if (isUpperCamel(doc.getInner().asScalar().getText())) {
        Ast::Type otype = doc.getOuter().getType();
        if (otype == Ast::Type::STMT_LIST || otype == Ast::Type::MEMBER_LIST) {
            doc.nestAsLeft(Ast::Type::DECL_STMT);
        }
    }
}

const char *IdentInputMode::name()
{
    return "Ident Input";
}

bool IdentInputMode::isUpperCamel(const std::string &id)
{
    // at least size 2, initial is upper, at least one lower
    return id.size() > 1 && isupper(id[0])
            && id.end() != std::find_if(id.begin() + 1, id.end(), islower);
}

