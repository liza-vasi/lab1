#include "file.h"
File::File(const QString &filePath, QObject *parent){
    setParent(parent);
    m_filePath = filePath;
    m_fileInfo =QFileInfo(filePath);
    m_lastExists =false;
    m_lastSize = 0;
}

 QString File::getFilePath() const
{
    return m_filePath;
}

bool File::exists()
{
    return m_fileInfo.exists();
}

qint64 File::getSize()
{
    return m_fileInfo.size();
}

void File::updateInfo()
{
    m_fileInfo.refresh();

}
