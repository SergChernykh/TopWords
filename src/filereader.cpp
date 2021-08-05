#include "filereader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QHash>
#include <algorithm>
#include <queue>
#include <QFileInfo>
#include <optional>
#include <QElapsedTimer>
#include <QThread>

FileReader::FileReader(QObject *parent)
    : QObject(parent)
    , m_heapCapacity(15){

}

void FileReader::processFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString errMessage = QString("Cannot open file by path %1").arg(path);
        qWarning() << errMessage;

        emit error(FileReaderError(std::move(errMessage)));
        return;
    }

    emit started();

//    QTextStream stream(&file);

    QString word;

    prepare();

    quint64 total = file.size();
    quint64 processed = 0;

    const int buffer_size = 4 * 1024;
    QByteArray buffer;

    QElapsedTimer timer;
    timer.start();

    while (!file.atEnd())
    {
        buffer = file.read(buffer_size);
        processed += buffer.size();

        QTextStream stream(&buffer);

        while (!stream.atEnd())
        {
            stream >> word;

            m_words[word].frequency++;

            auto& item = m_words[word];

            if (isHeapContainsItem(item))
            {
                updateWordFrequencyInHeap(item);
                emit newWord(word, item.frequency);
            }
            else
            {
                if (!isHeapFull())
                {
                    pushInHeap(word, item.frequency);
                    emit newWord(word, item.frequency);
                }
                else
                {
                    if (item.frequency > m_topWordsHeap.front().frequency)
                    {
                        const QString& oldWord = m_topWordsHeap.front().word;
                        m_words[oldWord].indexInHeap = INDEX_NONE;
                        emit removeWord(oldWord);
                        popFromHeap();
                        pushInHeap(word, item.frequency);
                        emit newWord(word, item.frequency);
                    }
                    else
                    {
                        continue;
                    }
                }
            }

            updateIndexesInHash();
        }

        emit progress(processed, total);


    }

    qDebug() << "total" << total;
    qDebug() << "processed" << processed;
    qDebug() << "elapsed time: " << timer.elapsed() << "ms";

    emit completed();

    file.close();
}

void FileReader::prepare()
{
    m_words.clear();
    m_topWordsHeap.clear();

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
