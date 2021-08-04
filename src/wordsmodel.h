#ifndef WORDSMODEL_H
#define WORDSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QTimer>

class WordsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Roles
    {
        Word = Qt::UserRole + 1,
        Frequency
    };

public:
    explicit WordsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void removeWord(const QString& word);
    void insertWord(const QString& word, int frequency);
    int count() const;

    void complete();

signals:
    void countChanged();

private:
    QMap<QString, int> m_wordsMap;
    QTimer m_tickTimer;
};

#endif // WORDSMODEL_H
