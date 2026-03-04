#ifndef FILE_H
#define FILE_H
#include <QObject>
#include <QFileInfo>
#include <QString>
//главная сущность

class File : public QObject
{
public:
    File(const QString &filePath, QObject *parent = nullptr);

    QString getFilePath()const;
    bool exists();
    qint64 getSize();
    void updateInfo();
private:
    QString m_filePath;
    QFileInfo m_fileInfo;
    bool m_lastExists;
    qint64 m_lastSize;
signals:
    void fileChange(const QString &filePath, bool exists, qint64 size);//общий сигнал

    void fileCreate(const QString &filePath, qint64 size);
    void fileDelete(const QString &filePath);
    void fileSizeChange(const QString &filePath, qint64 oldSize, qint64 newSize);
    void fileMod(const QString &filePath, qint64 size);
};

#endif // FILE_H
