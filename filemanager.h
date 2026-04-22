#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFileInfo>
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QMap>
class FileManager : public QObject
{
    Q_OBJECT

public:
    static FileManager& instance();
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

    void addFiles(const QStringList& filePaths);
    void deleteFile(const QString& filePath);
    void clear();
    void addFile(const QString& filePath);

    QStringList getWatchedFiles() const;
    int getFileCount() const;
    bool isWatching(const QString &filePath) const;

    void checkAllFiles();

signals:
    void fileCreate(const QString &filePath, qint64 size);
    void fileDelete(const QString &filePath);
    void fileChangeSize(const QString &filePath, qint64 oldSize, qint64 newSize);
    void fileModif(const QString &filePath, qint64 size);
    void fileChange(const QString &filePath, bool exists, qint64 size);

private:
    explicit FileManager(QObject *parent = nullptr);
    ~FileManager();
    struct FileState {
        bool lastExists;
        qint64 lastSize;
        QDateTime lastModif;

        FileState() : lastExists(false), lastSize(0) {}

        void update(bool exists, qint64 size, const QDateTime &modified) {
            lastExists = exists;
            lastSize = size;
            lastModif = modified;
        }
    };
    QMap<QString, FileState> m_files;
    QFileInfo getFileInfo(const QString &filePath) const;
};

#endif // FILEMANAGER_H
