#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void onMessageReceived(IrcPrivateMessage *message);
    ~MainWindow();

public slots:
    void addTab();
    void channelChanged();

private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void addTab(QString channel); //only for debugging

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
