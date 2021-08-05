#include "topcounter.h"

#include "wordsmodel.h"
#include <QDebug>

TopCounter::TopCounter(QObject *parent)
    : QObject(parent)
    , m_progress(0)
    , m_fileProcessing(false)
{
    m_reader = new FileReader();
    m_reader->moveToThread(&m_thread);

    connect(&m_thread, &QThread::finished, m_reader, &QObject::deleteLater);

    connect(m_reader, &FileReader::error, [this](const FileReaderError& e){
        emit error(e.message);
    });
    connect(m_reader, &FileReader::started, this, &TopCounter::onStarted);
    connect(m_reader, &FileReader::completed, this, &TopCounter::onCompleted);
    connect(m_reader, &FileReader::newWord, this, &TopCounter::onNewWord);
    connect(m_reader, &FileReader::progress, this, &TopCounter::onProgress);
    connect(m_reader, &FileReader::removeWord, this, &TopCounter::onRemoveWord);
    connect(this, &TopCounter::requestProcessFile, m_reader, &FileReader::processFile);

    m_thread.start();
}

TopCounter::~TopCounter()
{
    m_thread.quit();
    m_thread.wait();
}

void TopCounter::processFile(const QString &path)
{
    emit requestProcessFile(path);
}

void TopCounter::onNewWord(const QString &word, int frequency)
{
    if (m_wordsModel)
    {
        m_wordsModel->insertWord(word, frequency);
    }
}

void TopCounter::onRemoveWord(const QString &word)
{
    if (m_wordsModel)
    {
        m_wordsModel->removeWord(word);
    }
}

void TopCounter::onStarted()
{
    setFileProcessing(true);
}

void TopCounter::onCompleted()
{
    setFileProcessing(false);
}

void TopCounter::onProgress(qint64 processed, qint64 total)
{
    if (total == 0)
    {
        return;
    }

    m_progress = static_cast<double>(processed) / static_cast<double>(total);
    emit progressChanged();
}

void TopCounter::setWordsModel(WordsModel *model)
{
    m_wordsModel = model;
}

void TopCounter::setFileProcessing(bool value)
{
    if (m_fileProcessing != value)
    {
        m_fileProcessing = value;
        emit fileProcessingChanged();
    }
}
