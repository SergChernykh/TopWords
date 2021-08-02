#include "filereader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QHash>
#include <algorithm>
#include <queue>
#include <optional>

static constexpr int INDEX_NONE = -1;

struct WordHashItem
{
    unsigned int frequency = 0;
    int indexInHeap = INDEX_NONE;
};

struct WordHeapitem
{
    QString word;
    unsigned int frequency;
};

struct Comparator
{
    bool operator() (const WordHeapitem& left, const WordHeapitem& item2)
    {
        return left.frequency > item2.frequency;
    }
};


FileReader::FileReader(QObject *parent) : QObject(parent)
{

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

    QHash<QString, WordHashItem> words;

    QVector<WordHeapitem> topWordsHeap;
    std::make_heap(topWordsHeap.begin(), topWordsHeap.end(), Comparator {});

    const int heapCapacity = 15;

    topWordsHeap.reserve(heapCapacity);

    while (!stream.atEnd())
    {
        stream >> word;
        words[word].frequency++;

        auto& item = words[word];

        if (item.indexInHeap != INDEX_NONE)
        {
            topWordsHeap[item.indexInHeap].frequency = item.frequency;
            std::make_heap(topWordsHeap.begin(), topWordsHeap.end(), Comparator {});
        }
        else
        {
            if (topWordsHeap.size() < heapCapacity)
            {
                topWordsHeap.push_back(WordHeapitem {word, item.frequency});
                std::push_heap(topWordsHeap.begin(), topWordsHeap.end(), Comparator {});
            }
            else
            {
                if (item.frequency > topWordsHeap.front().frequency)
                {
                    words[topWordsHeap.front().word].indexInHeap = INDEX_NONE;
                    std::pop_heap(topWordsHeap.begin(), topWordsHeap.end(), Comparator {});
                    topWordsHeap.pop_back(); //TODO: may be use pop_front() instead

                    topWordsHeap.push_back(WordHeapitem {word, item.frequency});
                    std::push_heap(topWordsHeap.begin(), topWordsHeap.end(), Comparator {});

                    //TODO: may be use one call std::make_heap
                }
                else
                {
                    continue;
                }
            }
        }

        // update indexes in hash
        for (int i = 0; i < topWordsHeap.size(); ++i)
        {
            const auto& item = topWordsHeap[i];
            words[item.word].indexInHeap = i;
        }
    }

    for (auto it = topWordsHeap.begin(); it != topWordsHeap.end(); ++it)
    {
        qDebug() << it->word << it->frequency;
    }


    file.close();
}
