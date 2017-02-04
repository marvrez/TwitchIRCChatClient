#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ircmanager.h"
#include "Messages/message.h"

#include <QDesktopServices>
#include <QDebug>
#include <QTextDocumentFragment>
#include <QtWebEngine>
#include <QToolButton>
#include <QLabel>
#include <QTabWidget>
#include <QInputDialog>

IrcManager write, read;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(&read,
                     &IrcConnection::privateMessageReceived,
                     this,
                     &MainWindow::onMessageReceived);
    QObject::connect(this->ui->tabWidget,
                     &QTabWidget::tabBarClicked,
                     this,
                     &MainWindow::channelChanged);

    QToolButton *tb = new QToolButton(this);
    tb->setText("+");
    tb->setAutoRaise(true);
    connect(tb,
            SIGNAL(clicked()),
            this,
            SLOT(addTab()));

    ui->tabWidget->addTab(new QLabel("You can join channels by pressing <b>\"+\"</b>"), QString());
    //ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabsClosable(true);

    ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, tb);
    ui->tabWidget->tabBar()->tabButton(0,QTabBar::LeftSide)->resize(0,0);
    ui->tabWidget->setTabsClosable(true);

    qDebug() << "LUL";
    //ui->tabWidget->setMovable(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectToIrc() {
    read.connect();
    write.connect();
    addTab("#imaqtpie");
    //TODO: IMPLEMENT
}

void MainWindow::onMessageReceived(IrcPrivateMessage *message) {
    QList<Message*> *messages = read.getMessages(message->target());

    int maxMessages = 150; //twitch default
    Message *newMessage = Message::onMessage(message);
    messages->append(newMessage);
    qDebug() << newMessage->raw_message;
}

void MainWindow::addTab() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter channelname"),
                                         tr("Channel:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()) {
        QLabel *tab = new QLabel(this);
        QString tabName = QString(text);
        for(int i = 0; i < ui->tabWidget->count(); ++i) {
            if (ui->tabWidget->tabText(i) == tabName)
                return;
        }
        QString ircUserName = (tabName.startsWith("#")) ? tabName.toLower() : "#" + tabName.toLower();
        if(write.joinChannel(&ircUserName) && read.joinChannel(&ircUserName))
            ui->tabWidget->insertTab(ui->tabWidget->count() - 1, tab, tabName);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 2);
        channelChanged(ui->tabWidget->count() - 2);
    }
    //TODO: ADD JOIN CHANNEL
}

void MainWindow::addTab(const QString channel) {
    if(write.joinChannel(&channel) && read.joinChannel(&channel)) {
        QLabel *tab = new QLabel(this);
        ui->tabWidget->insertTab( ui->tabWidget->count() - 1, tab, channel.mid(1));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 2);
        channelChanged(ui->tabWidget->count() -2 );
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    if(ui->tabWidget->count() > 1 && index != ui->tabWidget->count()-1) {
        QString channelName = ui->tabWidget->tabText(index);

        write.removeChannel(channelName);
        read.removeChannel(channelName);

        this->curChannel = (ui->tabWidget->count() == 2) ? "" : ui->tabWidget->tabText(index - 1);
        ui->tabWidget->removeTab(index);
    }
    //TODO: also delete widget?
}

void MainWindow::channelChanged(int index) {
    this->curChannel =  ui->tabWidget->tabText(index);
    qDebug() <<  this->curChannel;
}
