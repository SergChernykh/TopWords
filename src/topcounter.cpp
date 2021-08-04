#include "topcounter.h"

#include <QDebug>

TopCounter::TopCounter(QObject *parent)
    : QObject(parent)
    , m_progress(0)
{
    m_reader = new FileReader();
    m_reader->moveToThread(&m_thread);

    connect(&m_thread, &QThread::finished, m_reader, &QObject::deleteLater);
    connect(&m_thread, &QThread::started, m_reader, &FileReader::procces);
    connect(m_reader, &FileReader::completed, this, &TopCounter::onCompleted);
    connect(m_reader, &FileReader::progress, this, &TopCounter::onProgress);

    m_thread.start();
}

TopCounter::~TopCounter()
{
    m_thread.quit();
    m_thread.wait();
}

void TopCounter::onNewWord(const QString &word)
{

}

void TopCounter::onCompleted()
{
    if (m_reader == nullptr)
    {
        return;
    }

    const auto& results = m_reader->getResults();

    for (const auto& item : results)
    {
        qDebug() << item.word << item.frequency;
    }
}

void TopCounter::onProgress(qint64 processed, qint64 total)
{
    if (total == 0)
    {
        return;
    }
    setProgress(static_cast<double>(processed) / static_cast<double>(total));
}

double TopCounter::progress() const
{
    return m_progress;
}

void TopCounter::setProgress(double newProgress)
{
    if (qFuzzyCompare(m_progress, newProgress))
        return;
    m_progress = newProgress;
    emit progressChanged();
}
