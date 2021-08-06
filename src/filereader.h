#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QHash>
#include <QVector>

inline constexpr int INDEX_NONE = -1;

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

struct FileReaderError
{
    QString message;

    FileReaderError() {}
    explicit FileReaderError(QString m) : message(std::move(m)) {}
};

Q_DECLARE_METATYPE(FileReaderError);

class FileReader : public QObject
{
    Q_OBJECT
public:
    explicit FileReader(QObject *parent = nullptr);

public slots:
    void processFile(const QString& path);

signals:
    void started();
    void completed();
    void progress(qint64 processed, qint64 total);
    void newWord(const QString& word, int frequency);
    void removeWord(const QString& word);
    void error(const FileReaderError& e);

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
