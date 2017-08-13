#ifndef COMPLETIONMANAGER_H
#define COMPLETIONMANAGER_H

#include <QAbstractListModel>
#include <QVector>

#include <string>

class CompletionModel : public QAbstractListModel {
public:
    virtual int columnCount(const QModelIndex&) const override {
        return 1;
    }

    virtual QVariant data(const QModelIndex& index, int role) const override {
        return QVariant(this->emotes.at(index.row()));
    }

    virtual int rowCount(const QModelIndex& parent) const override {
        return this->emotes.size();
    }

    void addString(const QString& str);

    QStringList emotes;
};

namespace CompletionManager {
CompletionModel* createModel(const QString& channelName);
} //namespace CompletionManager;


#endif // COMPLETIONMANAGER_H
