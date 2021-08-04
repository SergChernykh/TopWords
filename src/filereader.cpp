#include "filereader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QHash>
#include <algorithm>
#include <queue>
#include <QFileInfo>
#include <optional>

FileReader::FileReader(QObject *parent)
    : QObject(parent)
    , m_heapCapacity(15){

}

void FileReader::read(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file by path: " << path;
        return;
    }

    QTextStream stream(&file);

    QString word;

    prepare();

    quint64 total = file.size();
    quint64 processed = 0;

    while (!stream.atEnd())
    {
        stream >> word;

        m_words[word].frequency++;

        auto& item = m_words[word];

        if (isHeapContainsItem(item))
        {
            updateWordFrequencyInHeap(item);
        }
        else
        {
            if (!isHeapFull())
            {
                pushInHeap(word, item.frequency);
            }
            else
            {
                if (item.frequency > m_topWordsHeap.front().frequency)
                {
                    m_words[m_topWordsHeap.front().word].indexInHeap = INDEX_NONE;
                    popFromHeap();
                    pushInHeap(word, item.frequency);
                }
                else
                {
                    continue;
                }
            }
        }

        updateIndexesInHash();
    }

    emit progress(total, total);
    emit completed();

    file.close();
}

void FileReader::procces()
{
    read("test3.txt");
}

const QVector<WordHeapitem> &FileReader::getResults() const
{
    return m_topWordsHeap;
}

void FileReader::prepare()
{
    std::make_heap(m_topWordsHeap.begin(), m_topWordsHeap.end(), Comparator {});
    m_topWordsHeap.reserve(m_heapCapacity);
}

bool FileReader::isHeapContainsItem(const WordHashItem &item) const
{
    return item.indexInHeap != INDEX_NONE;
}

void FileReader::updateWordFrequencyInHeap(const WordHashItem &item)
{
    m_topWordsHeap[item.indexInHeap].frequency = item.frequency;
    std::make_heap(m_topWordsHeap.begin(), m_topWordsHeap.end(), Comparator {});
}

bool FileReader::isHeapFull() const
{
    return m_topWordsHeap.size() >= m_heapCapacity;
}

void FileReader::pushInHeap(const QString &word, unsigned int frequency)
{
    m_topWordsHeap.push_back(WordHeapitem {word, frequency});
    std::push_heap(m_topWordsHeap.begin(), m_topWordsHeap.end(), Comparator {});
}

void FileReader::popFromHeap()
{
    std::pop_heap(m_topWordsHeap.begin(), m_topWordsHeap.end(), Comparator {});
    m_topWordsHeap.pop_back();
}

void FileReader::updateIndexesInHash()
{
    for (int i = 0; i < m_topWordsHeap.size(); ++i)
    {
        const auto& item = m_topWordsHeap[i];
        m_words[item.word].indexInHeap = i;
    }
}
