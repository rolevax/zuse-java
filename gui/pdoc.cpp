#include "gui/pdoc.h"
#include <Qt>
#include <QKeySequence>
#include <QChar>

#include <QDebug>

PDoc::PDoc(QObject *parent) :
    QObject(parent),
    doc(new Doc(*this, *this))
{
}

void PDoc::load(QString filename)
{
    try {
        doc->load(filename.toStdString());
    } catch (const std::exception &e) {
        emit message(e.what());
    }
}

void PDoc::save(QString filename)
{
    try {
        doc->save(filename.toStdString());
    } catch (const std::exception &e) {
        emit message(e.what());
    }
}

void PDoc::keyboard(QString key, int modifier)
{
    (void) modifier;
    if (key.size() > 0)
        doc->keyboard(key.at(0).toLatin1());
}

void PDoc::observeTension(bool b)
{
    emit tension(b);
}

void PDoc::observePush(const char *name)
{
    emit pushed(name);
}

void PDoc::observePop()
{
    emit popped();
}

void PDoc::observeInsertLine(size_t r, size_t ct)
{
    emit insertLine(r, ct);
}

void PDoc::observeRemoveLine(size_t r, size_t ct)
{
    emit removeLine(r, ct);
}

void PDoc::observeUpdateLine(size_t r, const std::string &s)
{
    emit updateLine(r, QString::fromStdString(s));
}

void PDoc::observeLight(size_t lbr, size_t lbc,
                            size_t ler, size_t lec,
                            size_t hbr, size_t hbc,
                            size_t her, size_t hec)
{
    emit lighted(lbr, lbc, ler, lec, hbr, hbc, her, hec);
}

void PDoc::observeHotLight(ssize_t back)
{
    emit hotLighted(back);
}
