#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWebEnginePage>
#include <QDesktopServices>

class ChatPage : public QWebEnginePage
{
    Q_OBJECT

public:
    ChatPage(QObject* parent = 0) : QWebEnginePage(parent){qDebug() << "here";}

    bool acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool isMainFrame)
    {
         qDebug() << "acceptNavigationRequest("<<url << "," << type << "," << isMainFrame<<")";

        if (type == QWebEnginePage::NavigationTypeLinkClicked)
        {
            QDesktopServices::openUrl(url);
            return false;
        }
        return true;
    }
};
#endif // CHATPAGE_H
