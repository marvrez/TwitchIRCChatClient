#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QDialog>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QNetworkRequest>

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = 0);
    //bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame);
    ~ChatWidget();

private:
    Ui::ChatWidget *ui;
};

#endif // CHATWIDGET_H
