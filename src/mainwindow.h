#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ChatWidget/chatwidget.h"
#include "ircmanager.h"
#include "Messages/emotemanager.h"

#include <QMainWindow>
#include <IrcMessage>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void connectToIrc();
    void onPrivMessageReceived(IrcPrivateMessage *message);
    void addTab(const QString channel); //only for debugging
    ~MainWindow();

    static IrcManager write, read;

public slots:
    void addTab();
    void channelChanged(int index);

private slots:
    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    QString curChannel;
    QMap <QString, ChatWidget*> chatWindows;
};

#endif // MAINWINDOW_H
