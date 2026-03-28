#include "filemanager.h"

FileManager& FileManager::instance()
{
    static FileManager instance;
    return instance;
}

FileManager::FileManager(QObject *parent)
    : QObject(parent)
{
}

FileManager::~FileManager()
{
    clear();
}

void FileManager::addFile(const QString &filePath)
{
    if (!m_files.contains(filePath)) {
        FileState state;

        QFileInfo info(filePath);
        if (info.exists()) {
            state.update(true, info.size(), info.lastModified());
        }

        m_files[filePath] = state;

        QFileInfo currentInfo = getFileInfo(filePath);
        if (currentInfo.exists()) {
            emit fileChange(filePath, true, currentInfo.size());
        } else {
            emit fileChange(filePath, false, 0);
        }
    }
}

void FileManager::addFiles(const QStringList &filePaths)
{
    for (const QString &path : filePaths) {
        addFile(path);
    }
}

void FileManager::deleteFile(const QString &filePath)
{
    if (m_files.contains(filePath)) {
        m_files.remove(filePath);
    }
}

void FileManager::clear()
{
    m_files.clear();
}

QStringList FileManager::getWatchedFiles() const
{
    return m_files.keys();
}

int FileManager::getFileCount() const
{
    return m_files.size();
}

bool FileManager::isWatching(const QString &filePath) const
{
    return m_files.contains(filePath);
}

QFileInfo FileManager::getFileInfo(const QString &filePath) const
{
    return QFileInfo(filePath);
}

void FileManager::checkAllFiles()
{
    for (auto i = m_files.begin(); i != m_files.end(); ++i) {
        const QString &path = i.key();
        FileState &state = i.value();

        QFileInfo currentInfo = getFileInfo(path);
        bool currentExists = currentInfo.exists();
        qint64 currentSize = 0;
        QDateTime currentModified;

        if (currentExists) {
            currentSize = currentInfo.size();
            currentModified = currentInfo.lastModified();
        }

        if (currentExists != state.lastExists) {
            if (currentExists) {
                emit fileCreate(path, currentSize);
                emit fileChange(path, true, currentSize);
            } else {
                emit fileDelete(path);
                emit fileChange(path, false, 0);
            }
        }
        else if (currentExists) {
            if (currentSize != state.lastSize) {
                emit fileChangeSize(path, state.lastSize, currentSize);
                emit fileChange(path, true, currentSize);
            }
            else if (currentModified != state.lastModif) {
                emit fileModif(path, currentSize);
                emit fileChange(path, true, currentSize);
            }
        }

        state.update(currentExists, currentSize, currentModified);
    }
}
