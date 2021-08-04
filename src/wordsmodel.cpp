#include "wordsmodel.h"

#include <QDebug>

WordsModel::WordsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_tickTimer.setInterval(1000 / 120);

    connect(&m_tickTimer, &QTimer::timeout, [this]{
        emit dataChanged(index(0), index(count() - 1), { Frequency, Word });
    });

    m_tickTimer.start();
}

int WordsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return count();
}

QVariant WordsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= count())
    {
        return QVariant();
    }

    auto it = m_wordsMap.begin();
    it += index.row();

    switch (role)
    {
    case Word:
        return it.key();
    case Frequency:
        return it.value();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> WordsModel::roleNames() const
{
    return {
        { Word, "word" },
        { Frequency, "frequency"}
    };
}

void WordsModel::removeWord(const QString &word)
{
    auto it = m_wordsMap.find(word);
    if (it != m_wordsMap.end())
    {
        int idx = std::distance(m_wordsMap.begin(), it);
        beginRemoveRows(QModelIndex(), idx, idx);
        m_wordsMap.erase(it);
        endRemoveRows();
//        emit dataChanged(index(0), index(m_wordsMap.size()), {Word, Frequency});
//        endResetModel();
//        emit countChanged();
//        emit layoutChanged();
    }
}

void WordsModel::insertWord(const QString &word, int frequency)
{
    auto it = m_wordsMap.find(word);
    if (it != m_wordsMap.end())
    {
        it.value() = frequency;
        QModelIndex idx = index(std::distance(m_wordsMap.begin(), it));
        if (idx.isValid())
        {
//            emit dataChanged(idx, idx, { Frequency, Word });
        }
    }
    else
    {
        beginInsertRows(QModelIndex(), count(), count());
//        beginResetModel();
        m_wordsMap.insert(word, frequency);
        endInsertRows();
//        endResetModel();
//        emit dataChanged(index(0), index(m_wordsMap.size()), {Word, Frequency});

//        emit countChanged();
    }
}

int WordsModel::count() const
{
    return m_wordsMap.size();
}

void WordsModel::complete()
{
    int total = 0;

    for (auto it = m_wordsMap.begin(); it != m_wordsMap.end(); ++it)
    {
        total += it.value();
        qDebug() << it.key() << it.value();
    }
    qDebug() << "total" << total;
}
