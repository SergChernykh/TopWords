#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>

class FileReader : public QObject
{
    Q_OBJECT
public:
    explicit FileReader(QObject *parent = nullptr);

    void read(const QString& path);

signals:

};

#endif // FILEREADER_H
