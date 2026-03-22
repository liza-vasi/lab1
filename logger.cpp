#include "logger.h"

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

Logger::Logger(QObject *parent)
    : QObject(parent)
{
}

QString Logger::getCurrentTime() const
{
    return QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
}

void Logger::logMessage(const QString &message)
{
    std::cout << "[" << getCurrentTime().toStdString() << "] "<< message.toStdString() << std::endl;
}

void Logger::logEvent(const QString &eventType, const QString &filePath, qint64 size)
{
    QString message;
    if (size > 0) {
        message = QString("EVENT: %1 | File: %2 | Size: %3 bytes").arg(eventType, filePath, QString::number(size));
    } else {
        message = QString("EVENT: %1 | File: %2").arg(eventType, filePath);
    }
    logMessage(message);
}

void Logger::logStatus(const QString &filePath, bool exists, qint64 size)
{
    QString status = exists ? "EXISTS" : "NOT EXISTS";
    QString message = QString("STATUS: %1 | File: %2 | Size: %3 bytes").arg(status, filePath, QString::number(size));
    logMessage(message);
}

void Logger::logFileCreated(const QString &path, qint64 size)
{
    logEvent("FILE CREATED", path, size);
}

void Logger::logFileDeleted(const QString &path)
{
    logEvent("FILE DELETED", path);
}

void Logger::logFileSizeChanged(const QString &path, qint64 oldSize, qint64 newSize)
{
    QString message = QString("EVENT: SIZE CHANGED | File: %1 | Old: %2 bytes | New: %3 bytes")
                          .arg(path, QString::number(oldSize), QString::number(newSize));
    logMessage(message);
}

void Logger::logFileModified(const QString &path, qint64 size)
{
    logEvent("FILE MODIFIED", path, size);
}

void Logger::logFileExists(const QString &path, qint64 size)
{
    logStatus(path, true, size);
}

void Logger::logFileNotExists(const QString &path)
{
    logStatus(path, false, 0);
}

void Logger::onFileCreated(const QString &filePath, qint64 size)
{
    logFileCreated(filePath, size);
}

void Logger::onFileDeleted(const QString &filePath)
{
    logFileDeleted(filePath);
}

void Logger::onFileSizeChanged(const QString &filePath, qint64 oldSize, qint64 newSize)
{
    logFileSizeChanged(filePath, oldSize, newSize);
}

void Logger::onFileModified(const QString &filePath, qint64 size)
{
    logFileModified(filePath, size);
}

void Logger::onFileChanged(const QString &filePath, bool exists, qint64 size)
{
    if (exists) {
        logFileExists(filePath, size);
    } else {
        logFileNotExists(filePath);
    }
}
