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
    return QDateTime::currentDateTime().toString("hh:mm:ss");
}

void Logger::log(const QString &message)
{
    std::cout << "[" << getCurrentTime().toStdString() << "] "
              << message.toStdString() << std::endl;
}

void Logger::onFileCreated(const QString &filePath, qint64 size)
{
    log("event: created | " + filePath + " | size: " + QString::number(size) + " bytes");
}

void Logger::onFileDeleted(const QString &filePath)
{
    log("event: deleted | " + filePath);
}

void Logger::onFileSizeChanged(const QString &filePath, qint64 oldSize, qint64 newSize)
{
    log("event: size changed | " + filePath + " | old: " + QString::number(oldSize) + " | new: " + QString::number(newSize));
}

void Logger::onFileModified(const QString &filePath, qint64 size)
{
    log("event: modified | " + filePath + " | size: " + QString::number(size) + " bytes");
}

void Logger::onFileChanged(const QString &filePath, bool exists, qint64 size)
{
    if (exists) {
        log("status: exists | " + filePath + " | size: " + QString::number(size) + " bytes");
    } else {
        log("status: no exists | " + filePath);
    }
}
