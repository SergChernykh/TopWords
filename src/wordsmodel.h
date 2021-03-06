#ifndef WORDSMODEL_H
#define WORDSMODEL_H

#include <QAbstractListModel>
#include <QColor>

class WordsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Roles
    {
        Word = Qt::UserRole + 1,
        Frequency,
        Color
    };

    struct WordData
    {
        int frequency;
        QColor color;
    };

public:
    explicit WordsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void removeWord(const QString& word);
    void insertWord(const QString& word, int frequency);
    int count() const;

    void refresh();
    void clear();

signals:
    void countChanged();

private:
    QColor generateRandomColor() const;

private:
    QMap<QString, WordData> m_wordsMap;
};

#endif // WORDSMODEL_H
