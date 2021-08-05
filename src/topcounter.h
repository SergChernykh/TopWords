#ifndef TOPCOUNTER_H
#define TOPCOUNTER_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include "filereader.h"

class WordsModel;

class TopCounter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double progress MEMBER m_progress NOTIFY progressChanged)
    Q_PROPERTY(bool fileProcessing MEMBER m_fileProcessing WRITE setFileProcessing NOTIFY fileProcessingChanged)
    Q_PROPERTY(int frequencyAxisMax MEMBER m_frequencyAxisMax WRITE setfrequencyAxisMax NOTIFY frequencyAxisMaxChanged)

public:
    explicit TopCounter(QObject *parent = nullptr);
    ~TopCounter();

    Q_INVOKABLE void processFile(const QString& path);

    void setWordsModel(WordsModel* model);

    void setfrequencyAxisMax(int newfrequencyAxisMax);

signals:
    void progressChanged();
    void fileProcessingChanged();
    void requestProcessFile(const QString& path);
    void error(const QString& message);
    void frequencyAxisMaxChanged();

public slots:
    void setFileProcessing(bool value);

private slots:
    void onNewWord(const QString& word, int frequency);
    void onRemoveWord(const QString& word);

    void onStarted();
    void onCompleted();
    void onProgress(qint64 processed, qint64 total);

private:
    QThread m_thread;
    FileReader* m_reader;
    double m_progress;
    WordsModel* m_wordsModel;
    bool m_fileProcessing;
    int m_frequencyAxisMax;
    double m_frequencyAxisThreshold;
    double m_frequecnyAxisIncreaseRatio;
    QTimer m_tickTimer;
    int m_framePerSeconds;
};

#endif // TOPCOUNTER_H
