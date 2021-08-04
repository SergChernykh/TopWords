#ifndef TOPCOUNTER_H
#define TOPCOUNTER_H

#include <QObject>
#include <QThread>
#include "filereader.h"

class TopCounter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double progress READ progress WRITE setProgress NOTIFY progressChanged)

public:
    explicit TopCounter(QObject *parent = nullptr);
    ~TopCounter();

    double progress() const;
    void setProgress(double newProgress);

signals:
    void progressChanged();

public slots:

private slots:
    void onNewWord(const QString& word);
    void onCompleted();
    void onProgress(qint64 processed, qint64 total);


private:
    QThread m_thread;
    FileReader* m_reader;
    double m_progress;
};

#endif // TOPCOUNTER_H
