#include "wordsmodel.h"

#include <QDebug>
#include <QRandomGenerator>

WordsModel::WordsModel(QObject *parent)
    : QAbstractListModel(parent)
{

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
        return it.value().frequency;
    case Color:
        return it.value().color;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> WordsModel::roleNames() const
{
    return {
        { Word, "word" },
        { Frequency, "frequency"},
        { Color, "color" }
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
    }
}

void WordsModel::insertWord(const QString &word, int frequency)
{
    auto it = m_wordsMap.find(word);
    if (it != m_wordsMap.end())
    {
        it.value().frequency = frequency;
    }
    else
    {
        beginInsertRows(QModelIndex(), count(), count());
        m_wordsMap.insert(word, WordData { frequency, generateRandomColor() });
        endInsertRows();
    }
}

int WordsModel::count() const
{
    return m_wordsMap.size();
}

void WordsModel::refresh()
{
    emit dataChanged(index(0), index(count() - 1), { Frequency, Word, Color });
}

void WordsModel::clear()
{
    beginResetModel();
    m_wordsMap.clear();
    endResetModel();
}

QColor WordsModel::generateRandomColor() const
{
   return QColor::fromRgb(QRandomGenerator::global()->generate());
}

