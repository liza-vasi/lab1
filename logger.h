#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <iostream>

class Logger : public QObject
{
    Q_OBJECT

public:
    static Logger& instance();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void logMessage(const QString &message);
    void logEvent(const QString &eventType, const QString &filePath, qint64 size = 0);
    void logStatus(const QString &filePath, bool exists, qint64 size);

    void logFileCreated(const QString &path, qint64 size);
    void logFileDeleted(const QString &path);
    void logFileSizeChanged(const QString &path, qint64 oldSize, qint64 newSize);
    void logFileModified(const QString &path, qint64 size);
    void logFileExists(const QString &path, qint64 size);
    void logFileNotExists(const QString &path);

public slots:
    void onFileCreated(const QString &filePath, qint64 size);
    void onFileDeleted(const QString &filePath);
    void onFileSizeChanged(const QString &filePath, qint64 oldSize, qint64 newSize);
    void onFileModified(const QString &filePath, qint64 size);
    void onFileChanged(const QString &filePath, bool exists, qint64 size);

private:
    explicit Logger(QObject *parent = nullptr);
    QString getCurrentTime() const;
};


#endif // LOGGER_H
