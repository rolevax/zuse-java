#ifndef ZUSE_DOC_LISTENER_H
#define ZUSE_DOC_LISTENER_H

#include "token.h"

#include <qdebug.h>

class DocListener
{
public:
    DocListener() = default;

    virtual void onLineInserted(size_t r, size_t ct)
    {
        (void) r; (void) ct;
    }

    virtual void onLineRemoved(size_t r, size_t ct)
    {
        (void) r; (void) ct;
    }

    virtual void onLineUpdated(size_t r, const std::string &s)
    {
        (void) r; (void) s;
    }

    virtual void onHotLight(ssize_t back)
    {
        (void) back;
    }

    virtual void onLight(size_t lbr, size_t lbc,
                         size_t ler, size_t lec,
                         size_t hbr, size_t hbc,
                         size_t her, size_t hec)
    {
        (void) lbr; (void) lbc;
        (void) ler; (void) lec;
        (void) hbr; (void) hbc;
        (void) her; (void) hec;
    }

    virtual void onCursorTension(bool b)
    {
        (void) b;
    }

    virtual void onModePushed(const char *name)
    {
        (void) name;
    }

    virtual void onModePopped()
    {
    }

    virtual void onClipSwitched(char c)
    {
        (void) c;
    }
};

#endif // ZUSE_DOC_LISTENER_H
