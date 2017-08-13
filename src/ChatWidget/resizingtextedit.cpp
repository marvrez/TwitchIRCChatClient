#include "ChatWidget/resizingtextedit.h"

ResizingTextEdit::ResizingTextEdit() {
}

ResizingTextEdit::ResizingTextEdit(QWidget* parent) {
    auto sizePolicy = this->sizePolicy();
    sizePolicy.setHeightForWidth(true);
    sizePolicy.setVerticalPolicy(QSizePolicy::Preferred);
    this->setSizePolicy(sizePolicy);
    this->setAcceptRichText(true);

    QObject::connect(this, &QTextEdit::textChanged, this, &QWidget::updateGeometry);

    this->setFocusPolicy(Qt::ClickFocus);
}

QSize ResizingTextEdit::sizeHint() const {
    return QSize(this->width(), this->heightForWidth(this->width()));
}

bool ResizingTextEdit::hasHeightForWidth() const {
    return true;
}

int ResizingTextEdit::heightForWidth(int) const {
    auto margins = this->contentsMargins();
    return margins.top() + document()->size().height() + margins.bottom() + 5;
}

QString ResizingTextEdit::textUnderCursor(bool* hadSpace) const {
    auto currentText = this->toPlainText();

    QTextCursor cursor = this->textCursor();

    auto textUpToCursor = currentText.left(cursor.selectionStart());

    auto words = textUpToCursor.splitRef(' ');
    if (words.size() == 0) return QString();

    bool first = true;
    QString lastWord;
    for (int i = words.size() - 1; i >= 0; --i) {
        auto word = words.at(i);

        if (first && word.isEmpty()) {
            first = false;
            if (hadSpace != nullptr) *hadSpace = true;
            continue;
        }

        lastWord = word.toString();
        break;
    }

    return lastWord.isEmpty() ? QString() : lastWord;
}

void ResizingTextEdit::keyPressEvent(QKeyEvent *event) {
    event->ignore();

    if (event->key() == Qt::Key_Tab) {
        QString currentCompletionPrefix = this->textUnderCursor();

        if (!currentCompletionPrefix.size()) return;

        if (!this->nextCompletion) {
            // first selection
            this->completer->setCompletionPrefix(currentCompletionPrefix);
            this->nextCompletion = true;
            this->completer->complete();
            return;
        }

        // scrolling through selections
        if (!this->completer->setCurrentRow(this->completer->currentRow() + 1)) {
            // wrap over and start again
            this->completer->setCurrentRow(0);
        }

        this->completer->complete();
        return;
    }

    else if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && event->modifiers() != Qt::ShiftModifier) {
        QString text = this->toPlainText();
        if(!text.isEmpty()) prevMsg.append(text);

        unsigned size = prevMsg.size();
        if(size > 1 && prevMsg.at(size - 1) == prevMsg.at(size - 2))
            prevMsg.pop_back();

        emit returnPressed();

        if (!(event->modifiers() == Qt::ControlModifier)) {
            this->setText(QString());
            prevIndex = 0;
        }
        prevIndex = prevMsg.size();
        return;
    }

    else if (event->key() == Qt::Key_Up) {
        if (prevMsg.size() && prevIndex) {
            this->setText(prevMsg.at(--prevIndex));
        }
    } else if (event->key() == Qt::Key_Down) {
        //if(!this->textCursor().atBlockEnd()) return;

        if (prevIndex != (prevMsg.size() - 1) && prevIndex != prevMsg.size()) {
            this->setText(prevMsg.at(++prevIndex));
        }
        else {
            prevIndex = prevMsg.size();
            this->setText(QString());
        }
    }

    // this resets the selection in the completion list, it should probably only trigger on actual chat input (space, character)
    this->nextCompletion = false;

    if (!event->isAccepted())
        QTextEdit::keyPressEvent(event);
}

void ResizingTextEdit::setCompleter(QCompleter* c) {
    if (this->completer) QObject::disconnect(this->completer, 0, this, 0);

    this->completer = c;
    if (!this->completer) return;

    this->completer->setWidget(this);
    this->completer->setCompletionMode(QCompleter::InlineCompletion);
    this->completer->setCaseSensitivity(Qt::CaseInsensitive);

    QObject::connect(completer,
                     static_cast<void (QCompleter::*)(const QString &)>(&QCompleter::highlighted),
                     this, &ResizingTextEdit::insertCompletion);
}

void ResizingTextEdit::insertCompletion(const QString& completion) {
    if (this->completer->widget() != this) return;

    bool hadSpace = false;
    auto prefix = this->textUnderCursor(&hadSpace);

    int prefixSize = prefix.size();

    if (hadSpace) ++prefixSize;

    auto cursor = this->textCursor();
    cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, prefixSize);
    cursor.insertText(completion);
    this->setTextCursor(cursor);
}

QCompleter* ResizingTextEdit::getCompleter() const {
    return this->completer;
}
