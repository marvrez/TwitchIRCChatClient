#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QtCore>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEngineUrlRequestInterceptor>

#define API_VERSION 5

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow){
    ui->setupUi(this);

    this->setWindowTitle("Login");

    webView = new QWebEngineView;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "marvin", "chat");
    this->ui->cbAutoConnect->setChecked(settings.value("Main/auto_connect", false).toBool());
    this->ui->username->setText(settings.value("Credentials/username").toString());
    this->ui->oauth->setText(settings.value("Credentials/password").toString());


    QObject::connect(this->ui->btnLogin, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(this->webView, SIGNAL(destroyed(QObject*)), this, SLOT(accept()));
}

LoginWindow::~LoginWindow(){
    delete ui;
}

void LoginWindow::on_btnLogin_clicked(){
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "marvin", "chat");
    settings.setValue("Main/auto_connect", this->ui->cbAutoConnect->isChecked());
    settings.setValue("Credentials/username", this->ui->username->text());
    settings.setValue("Credentials/password", this->ui->oauth->text());
    if(webView->isVisible()) webView->close();

    this->hide();
    this->exec();
}

void LoginWindow::on_btnTwitchLogin_clicked(){
     this->webView->close();

     QUrl url = QUrl(QString("https://api.twitch.tv/kraken/oauth2/authorize"
                             "?response_type=token"
                             "&client_id=%1"
                             "&redirect_uri=%2"
                             "&scope=user_read+chat_login"
                             "&force_verify=true").arg(clientID, redirectURL));
     //qDebug() << url;
     webView->load(url);
     webView->setWindowTitle("Login with Twitch");
     webView->show();

     QObject::connect(webView, &QWebEngineView::urlChanged, [&]() {
         qDebug() << "Url being viewed:" << webView->url();
         QRegularExpression url_regex = QRegularExpression("\\b(https|http):\/\/www.twitch.tv\/deiityy");
         QRegularExpression oauth_regex = QRegularExpression("(access_token=?)([a-z0-9]*)");

         if(url_regex.match(webView->url().toString()).hasMatch()) {
                webView->close();
                if(oauth_regex.match(webView->url().toString()).hasMatch()) {
                     QString raw_oauth(oauth_regex.match(webView->url().toString()).captured());
                     QString oauth = raw_oauth.mid(raw_oauth.indexOf("=")+1);

                     QSettings settings(QSettings::IniFormat, QSettings::UserScope, "marvin", "chat");
                     settings.setValue("Main/auto_connect", this->ui->cbAutoConnect->isChecked());
                     settings.setValue("Credentials/password", "oauth:" + oauth);

                     QUrl url = QUrl(QString("https://api.twitch.tv/kraken?oauth_token=%1&api_version=%2").arg(oauth).arg(API_VERSION));
                     qDebug() << url;
                     webView->load(QUrl("")); //idk whyk... but it works
                     QNetworkRequest req(url);
                     QNetworkReply *reply = this->network_access_manager.get(req);

                     connect(reply, &QNetworkReply::finished, [=]() {
                             QByteArray data = reply->readAll();
                             QJsonDocument jsonDoc(QJsonDocument::fromJson(data));

                             QJsonObject root = jsonDoc.object();
                             QJsonObject token = root.value("token").toObject();

                             QString username = token.value("user_name").toString();
                             QSettings settings(QSettings::IniFormat, QSettings::UserScope, "marvin", "chat");
                             settings.setValue("Credentials/username", username);
                             emit accept();
                         });
                }
         }
     });
     //webView->deleteLater();
}

void LoginWindow::on_cbAutoConnect_stateChanged(int state)
{
    qDebug() << "Check button changed to state:" << state;
}

void LoginWindow::on_cbShowOauth_stateChanged(int state) {
    qDebug() << "Show OAuth-state changed to state:" << state;
    if(state == Qt::CheckState::Checked) this->ui->oauth->setEchoMode(QLineEdit::Normal);
    else this->ui->oauth->setEchoMode(QLineEdit::Password);
}

