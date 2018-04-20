#ifndef PDOC_H
#define PDOC_H

#include <QObject>
#include <memory>
#include "libzuse/zuse.h"

class PDoc : public QObject, public TokensObserver
{
    Q_OBJECT
public:
    explicit PDoc(QObject *parent = 0);
    Q_INVOKABLE void load(QString filename);
    Q_INVOKABLE void save(QString filename);
    Q_INVOKABLE void keyboard(QString key, int modifier);

    void observeTension(bool b);
    void observePush(const char *name);
    void observePop();

    void observeSwitchClip(char c);

    void observeInsertLine(size_t r, size_t ct) override;
    void observeRemoveLine(size_t r, size_t ct) override;
    void observeUpdateLine(size_t r, const std::string &s) override;
    void observeLight(size_t lbr, size_t lbc,
                      size_t ler, size_t lec,
                      size_t hbr, size_t hbc,
                      size_t her, size_t hec) override;
    void observeHotLight(ssize_t back) override;

signals:
    void message(QString text);
    void tension(bool b);
    void pushed(QString name);
    void popped();

    void clipSwitched(char c);

    void insertLine(int r, int ct);
    void removeLine(int r, int ct);
    void updateLine(int r, const QString str);
    void hotLighted(int back);
    void lighted(int lbr, int lbc, int ler, int lec,
                 int hbr, int hbc, int her, int hec);

public slots:

private:
    std::unique_ptr<Doc> mDoc;
};

#endif // PDOC_H
