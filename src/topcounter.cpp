#include "topcounter.h"

#include "wordsmodel.h"
#include <QDebug>
#include <cmath>

TopCounter::TopCounter(QObject *parent)
    : QObject(parent)
    , m_progress(0)
    , m_fileProcessing(false)
    , m_frequencyAxisMax(100)
    , m_frequencyAxisThreshold(0.95)
    , m_frequecnyAxisIncreaseRatio(2.0)
    , m_framePerSeconds(120)
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

    m_tickTimer.setInterval(1000 / m_framePerSeconds);
    connect(&m_tickTimer, &QTimer::timeout, [this]{
        if (m_wordsModel != nullptr)
        {
            m_wordsModel->refresh();
        }
    });
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
        if (frequency > m_frequencyAxisMax * m_frequencyAxisThreshold)
        {
            setfrequencyAxisMax(m_frequencyAxisMax * m_frequecnyAxisIncreaseRatio);
        }
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
    m_tickTimer.start();
}

void TopCounter::onCompleted()
{
    setFileProcessing(false);
    m_tickTimer.stop();
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

void TopCounter::setfrequencyAxisMax(int newfrequencyAxisMax)
{
    if (m_frequencyAxisMax == newfrequencyAxisMax)
        return;
    m_frequencyAxisMax = newfrequencyAxisMax;
    emit frequencyAxisMaxChanged();
}
