#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QCheckBox>
#include <QWebView>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QNetworkAccessManager>


namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

    private slots:
        void on_btnLogin_clicked();

        void on_btnTwitchLogin_clicked();

        void on_cbAutoConnect_stateChanged(int state);

        void on_cbShowOauth_stateChanged(int state);

private:
    Ui::LoginWindow *ui;
    const QString clientID = QString("hmcgc5sv91fyzjsj5ekostolzcxkcg");
    const QString redirectURL = QString("https://twitch.tv/deiityy");
    QNetworkAccessManager network_access_manager;
    QWebView *webView;
    bool finished = false;

    void changeState(bool state);
    //bool close();

};

#endif // LOGINWINDOW_H
