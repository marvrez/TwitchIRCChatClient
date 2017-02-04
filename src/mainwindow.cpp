#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ircmanager.h"

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

    addTab("#imaqtpie");
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

void MainWindow::onMessageReceived(IrcPrivateMessage *message) {
    qDebug() << message->toData();
}

void MainWindow::addTab() {
    QLabel *tab = new QLabel(this);
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter channelname"),
                                         tr("Channel:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()) {
        QString tabName = QString(text);
        for(int i = 0; i < ui->tabWidget->count(); ++i) {
            if (ui->tabWidget->tabText(i) == tabName)
                return;
        }
        QString ircUserName = (tabName.startsWith("#")) ? tabName.toLower() : "#" + tabName.toLower();
        if(write.joinChannel(&ircUserName) && read.joinChannel(&ircUserName))
            ui->tabWidget->insertTab( ui->tabWidget->count() - 1, tab, tabName);
    }
    //TODO: ADD JOIN CHANNEL
}

void MainWindow::addTab(const QString channel) {
        QLabel *tab = new QLabel(this);
        if(write.joinChannel(&channel) && read.joinChannel(&channel))
            ui->tabWidget->insertTab( ui->tabWidget->count() - 1, tab, channel);
}

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    if(ui->tabWidget->count() > 1 && index != ui->tabWidget->count()-1) {
        IrcCommand *part =  IrcCommand::createPart(ui->tabWidget->tabText(index));
        write.sendCommand(part);
        read.sendCommand(part);
        ui->tabWidget->removeTab(index);
    }
    //TODO: also delete widget?
}

void MainWindow::channelChanged() {

}
