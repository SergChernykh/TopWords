#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QHash>
#include <QVector>

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

//TODO: rename
class FileReader : public QObject
{
    Q_OBJECT
public:
    explicit FileReader(QObject *parent = nullptr);

    void read(const QString& path);
    void procces();
    const QVector<WordHeapitem>& getResults() const;

signals:
    void completed();
    void progress(qint64 processed, qint64 total);
    void newWord(const QString& word, int frequency);
    void removeWord(const QString& word);

private:
    void prepare();
    bool isHeapContainsItem(const WordHashItem& item) const;
    void updateWordFrequencyInHeap(const WordHashItem& item);
    bool isHeapFull() const;
    void pushInHeap(const QString& word, unsigned int frequency);
    void popFromHeap();
    void updateIndexesInHash();

private:
    QHash<QString, WordHashItem> m_words;
    QVector<WordHeapitem> m_topWordsHeap;
    int m_heapCapacity;
};

#endif // FILEREADER_H
