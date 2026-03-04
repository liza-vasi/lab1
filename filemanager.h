#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "file.h"
//отвечет за добавление и исключение из под наблюдение файлов
class FileManager : public QObject
{
public:
    FileManager(QObject *parent = nullptr);
    ~FileManager();
    void addFiles(const QStringList& filePath);
    void deleteFile(const QString& filePath);
    void start(int lenTime= 100);
    void stop();

    void addFile(const QString& filePath);

    QStringList getFileWatch();
private:
    File* file;
    QList<File*> m_files;
private slots:
    void checkFiles();
};

#endif // FILEMANAGER_H
