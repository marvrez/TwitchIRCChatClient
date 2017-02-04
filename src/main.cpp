#include "mainwindow.h"
#include "LoginWindow/loginwindow.h"
#include <QApplication>
#include <QSettings>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists()) {
        printf("Unable to set stylesheet, file not found\n");
    } else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "marvin", "chat");

    int r = 0;
    MainWindow w;

    if (settings.value("Main/auto_connect", false).toBool())
        w.connectToIrc();
    else {
        LoginWindow lw;
        lw.show();
        r = lw.exec();

        if (r == QDialog::Accepted) {
            w.connectToIrc();
            qDebug() << settings.value("Credentials/username","") << settings.value("Credentials/password","");
        } else if (r == QDialog::Rejected)
            return 0;
    }
    w.show();

    return app.exec();
}
