#ifndef SCALARINPUTMODE_H
#define SCALARINPUTMODE_H

#include "mode.h"

class ScalarInputMode : public Mode
{
public:
    ScalarInputMode(EditableDoc &doc);

    void keyboard(char key) override;
    void onPushed() override;
//    void onPopped() override;
    const char *name() override;
};

#endif // SCALARINPUTMODE_H
