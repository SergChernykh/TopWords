#ifndef WORDSMODEL_H
#define WORDSMODEL_H

#include <QObject>
#include <QAbstractListModel>

class WordsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WordsModel(QObject *parent = nullptr);

signals:

};

#endif // WORDSMODEL_H
