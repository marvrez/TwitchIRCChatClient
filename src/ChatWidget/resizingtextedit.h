#ifndef RESIZINGTEXTEDIT_H
#define RESIZINGTEXTEDIT_H

#include "chatwidget.h"

#include <QTextEdit>
#include <QKeyEvent>
#include <QCompleter>

class ResizingTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    ResizingTextEdit();
    explicit ResizingTextEdit(QWidget* parent = 0);

    QSize sizeHint() const override;
    bool hasHeightForWidth() const override;

    void setCompleter(QCompleter* c);
    QCompleter* getCompleter() const;

signals:
    void returnPressed();

protected:
    int heightForWidth(int) const override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QCompleter* completer = nullptr;
    QWidget* parent;

    // hadSpace is set to true in case the "textUnderCursor" word was after a space
    QString textUnderCursor(bool* hadSpace = nullptr) const;

    bool nextCompletion = false;

    unsigned prevIndex = 0;
    QStringList prevMsg; //stack for sent messages

private slots:
    void insertCompletion(const QString& completion);
};

#endif // RESIZINGTEXTEDIT_H
