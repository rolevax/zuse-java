#include "core/editabledoc.h"
#include "mode/pairinputmode.h"
#include "mode/menumode.h"
#include "mode/stringinputmode.h"

#include <iostream>

PairInputMode::PairInputMode(EditableDoc &doc)
    : Mode(doc)
{

}

void PairInputMode::onPushed()
{
    doc.fallIn();
    stage = Stage::KEY_DONE;
    doc.push(new StringInputMode(doc, true));
}

void PairInputMode::onResume()
{
    switch (stage) {
    case Stage::KEY_DONE:
        doc.sibling(+1);
        stage = Stage::VALUE_DONE;
        //doc.push(new MenuMode(doc, MenuMode::Context::CHANGE));
        break;
    case Stage::VALUE_DONE:
        doc.digOut();
        doc.pop();
        break;
    }
}

const char *PairInputMode::name()
{
    return "Pair Input";
}
