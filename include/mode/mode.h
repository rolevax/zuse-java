#ifndef MODE_H
#define MODE_H

#include "ast/ast.h"

class EditableDoc;

class Mode
{
public:
    struct Result
    {
        bool pop;
        bool handled;
        Mode *nextPush;
    };

    Mode(const Mode& copy) = delete;
    Mode &operator=(const Mode& assign) = delete;
    virtual ~Mode() = default;

    virtual Result keyboard(char key, bool top)
    {
        (void) key;
        (void) top;
        return { false, false, nullptr };
    }

    virtual Result onPushed()  { return { false, true, nullptr }; }
    virtual void onPopped() {}
    virtual void onResume() {}
    virtual const char *name() = 0;

protected:
    Mode(EditableDoc &doc) : doc(doc) {}
    EditableDoc &doc;
};

#endif // MODE_H
