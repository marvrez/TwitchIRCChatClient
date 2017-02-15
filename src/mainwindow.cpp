#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ircmanager.h"
#include "Messages/message.h"
#include "ChatWidget/chatwidget.h"

#include <QDesktopServices>
#include <QDebug>
#include <QTextDocumentFragment>
#include <QWebElement>
#include <QToolButton>
#include <QLabel>
#include <QTabWidget>
#include <QScrollBar>
#include <QInputDialog>
#include <QTabWidget>
#include <QDir>

IrcManager MainWindow::write;
IrcManager MainWindow::read;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    this->setWindowTitle("Twitch chat client");
    this->ui->tabWidget->setStyleSheet("QTabBar::tab {background-color:#27252D; font-family: \"Arial\", Times, serif;}"
                                       "QTabBar::tab::selected, QTabBar::tab::hover {background-color:#17141F; font: bold;}");
    QObject::connect(&read,
                     &IrcConnection::privateMessageReceived,
                     this,
                     &MainWindow::onPrivMessageReceived);
    QObject::connect(this->ui->tabWidget,
                     &QTabWidget::tabBarClicked,
                     this,
                     &MainWindow::channelChanged);

    QToolButton *tb = new QToolButton();
    tb->setText("+");
    tb->setAutoRaise(true);
    QObject::connect(tb,
            SIGNAL(clicked()),
            this,
            SLOT(addTab()));

    ui->tabWidget->addTab(new QLabel("You can join channels by pressing <b>\"+\"</b>"), QString());
    //ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabsClosable(true);

    ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, tb);
    ui->tabWidget->tabBar()->tabButton(0,QTabBar::LeftSide)->resize(0,0);
    ui->tabWidget->setTabsClosable(true);

    //ui->tabWidget->setMovable(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectToIrc() {
    read.connect();
    write.connect();
    //TODO: IMPLEMENT
}

void MainWindow::onPrivMessageReceived(IrcPrivateMessage *message) {
    QList<Message*> *messages = read.getMessages(message->target());

    auto channelStates = chatWindows[this->curChannel]->getChannelStates();
    Message *newMessage = Message::onMessage(message, channelStates);
    messages->append(newMessage);
    if(message->target() == this->curChannel) {
        chatWindows[this->curChannel]->updateMessageScreen(messages);
        chatWindows[this->curChannel]->addMessage(newMessage);
    }
    else
        chatWindows[this->curChannel]->tryRemoveFirstMessage(messages);
}

void MainWindow::addTab() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter channelname"),
                                         tr("Channel:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()) {
        text = text.trimmed();
        QString ircUserName = (text.startsWith("#")) ? text.toLower() : "#" + text.toLower();

        if(write.joinChannel(&ircUserName) && read.joinChannel(&ircUserName)){
            if(!chatWindows.contains(ircUserName)) chatWindows[ircUserName] = new ChatWidget(this);
            chatWindows[ircUserName]->setChannel(ircUserName);
            this->ui->tabWidget->insertTab(ui->tabWidget->count() - 1, chatWindows[ircUserName], text.toLower());
        }

        this->ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 2);
        channelChanged(ui->tabWidget->count() - 2);
    }
}

void MainWindow::addTab(const QString channel) {
    QString ircUserName = (channel.startsWith("#")) ? channel.toLower() : "#" + channel.toLower();
    if(read.joinChannel(&ircUserName) && write.joinChannel(&ircUserName)) {
        QLabel *tab = new QLabel(this);
            ui->tabWidget->insertTab(ui->tabWidget->count() - 1, tab, channel.mid(1));
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 2);
            channelChanged(ui->tabWidget->count() - 2);
        }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    if(ui->tabWidget->count() > 1 && index != ui->tabWidget->count()-1) {
        QString channelName = ui->tabWidget->tabText(index);

        write.removeChannel(channelName);
        read.removeChannel(channelName);

        this->curChannel = (ui->tabWidget->count() == 2) ? "" : ui->tabWidget->tabText(index - 1);
        ui->tabWidget->widget(index)->close();
        ui->tabWidget->removeTab(index);
        ui->tabWidget->setCurrentIndex(index-1);
        MainWindow::channelChanged(this->ui->tabWidget->currentIndex());
    }
    //TODO: also delete widget?
}

void MainWindow::channelChanged(int index) {
    qDebug() << (this->curChannel != "#"+ui->tabWidget->tabText(index));
    if(index != ui->tabWidget->count()-1 && this->curChannel != "#"+ui->tabWidget->tabText(index)) {
        this->curChannel = "#" + ui->tabWidget->tabText(index);

        chatWindows[this->curChannel]->channelChanged(read.getMessages(this->curChannel));
        qDebug() <<  this->curChannel;
    }
}
