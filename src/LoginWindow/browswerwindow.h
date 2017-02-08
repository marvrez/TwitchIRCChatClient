#ifndef BROWSWERWINDOW_H
#define BROWSWERWINDOW_H

#include <QWebView>

class BrowswerWindow : QWidget
{
public:
    BrowswerWindow(QWidget *parent = nullptr);
    void setUrl(QUrl url);
    void loadPage();
private:
    QWebEngineView *view;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // BROWSWERWINDOW_H
