#include "browswerwindow.h"

#include <QWebEngineView>
#include <QUrl>
BrowswerWindow::BrowswerWindow(QWidget *parent){
    view = new QWebEngineView(nullptr);
}

void BrowswerWindow::setUrl(QUrl url) {
    view->load(url);
}

void BrowswerWindow::loadPage() {
    view->show();
}

void BrowswerWindow::closeEvent(QCloseEvent *event) {
    this->hide();
}
