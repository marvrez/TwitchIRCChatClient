#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "chatpage.h"

#include <QFileInfo>
#include <QDir>

ChatWidget::ChatWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    this->layout()->setContentsMargins(0,0,0,0);

    this->ui->chatWindow->setPage(new ChatPage);
    this->ui->chatWindow->setUrl( QUrl("qrc:/ChatWindow/index.html"));
}

ChatWidget::~ChatWidget()
{
    delete ui;
}
