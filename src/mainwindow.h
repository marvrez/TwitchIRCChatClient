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
    void addTab(const QString channel); //only for debugging
    ~MainWindow();

public slots:
    void addTab();
    void channelChanged(int index);

private slots:
    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    QString curChannel;
};

#endif // MAINWINDOW_H
