Васильева Елизавета
932321

# 1. Постановка задачи
Необходимо разработать программу с консольным интерфейсом, которая выполняет слежение за выбранными файлами в реальном времени.
Основные требования:
1.	Отслеживание существования файла;
2.	Отслеживание размера файла;
3.	Отслеживание даты последней модификации;
4.	Вывод уведомлений о произошедших изменениях в консоль с временной меткой.

# 2. Предлагаемое решение
## 2.1 Общая идея
Программа реализует опросный механизм мониторинга: с заданным интервалом проверяются данные файлов и сравниваются с ранее сохранёнными значениями. При обнаружении расхождений генерируются события, которые и выводятся в лог.
При решении будем использовать сигнально-слотовые соединения и паттерн проектирования Singleton. Singleton (одиночка) - для классов FileManager и Logger обеспечивает единственный экземпляр каждого класса

## 2.2 Описание классов
### 1. FileManager
instance() - Получение единственного экземпляра (Singleton)

addFile() - Добавление файлов в список наблюдения

addFiles() - Добавление файлов в список наблюдения

checkAllFiles() - Основная логика: сравнение текущего и сохранённого состояния

FileState (struct) - Хранение данных: существование, размер, дата изменения

deleteFile(const QString&) - удаление из списка отслеживаемых

clear() - очистка всего списка

getWatchedFiles() - получение списка отслеживаемых файлов

getFileCount() - получение количества отслеживаемых файлов

isWatching(const QString&) - проверка, отслеживается ли файл

**Сигналы:**

fileCreate(QString, qint64) - файл создан

fileDelete(QString) - файл удалён

fileChangeSize(QString, qint64, qint64) - размер изменён

fileModif(QString, qint64) - файл модифицирован

fileChange(QString, bool, qint64) - общее изменение статуса



### 2. Класс Logger
instance() - статический метод для получения единственного экземпляра;
log(const QString&) - базовый вывод сообщения с временной меткой;
getCurrentTime() - получение текущего времени в формате ЧЧ:ММ:СС.

Слоты:
onFileCreated(QString, qint64) - обработка создания файла;
onFileDeleted(QString) - обработка удаления файла;
onFileSizeChanged(QString, qint64, qint64) - обработка изменения размера;
onFileModified(QString, qint64) - обработка модификации файла;
onFileChanged(QString, bool, qint64) - обработка общего изменения статуса.
## 2.3 UML диаграмма
https://drive.google.com/file/d/1a69oOEt5hJQszdIylOWRG-ithZRSluIQ/view?usp=sharing 
# 3. Инструкция пользователя

## 3.1 Настройка программы
Аргументы командной строки:
- `--interval=….` - интервал проверки в миллисекундах (по умолчанию 100 мс)
- Остальные аргументы - пути к файлам для мониторинга

## 3.2 Примеры запуска

**Мониторинг одного файла с интервалом 1 секунда:**
- `./file-watcher --interval=1000 /home/user/document.txt`

**Мониторинг нескольких файлов с интервалом по умолчанию:**
- `./file-watcher /home/user/file1.txt /home/user/file2.txt`

## 3.3 Пример работы программы

**Запуск программы:**
- `--interval=100 C:/Users/User/Desktop/FileW/file1.txt C:/Users/User/Desktop/FileW/file2.txt`

**Вывод программы:**

- `[12:38:03] File watcher`
- `[12:38:03] Check interval: 100 ms`
- `[12:38:03] Watched files:`
- `[12:38:03]   C:/Users/User/Desktop/FileW/file1.txt`
- `[12:38:03]   C:/Users/User/Desktop/FileW/file2.txt`
- `[12:38:03] Initial state:`
- `[12:38:03] status: exists |C:/Users/User/Desktop/FileW/file1.txt | size: 5 bytes`
- `[12:38:03] status: exists |C:/Users/User/Desktop/FileW/file2.txt | size: 4 bytes`

**Изменение размера файла test1.txt:**
- `echo "..." >> file1.txt`

**Вывод программы:**
- `[12:39:57] event: size changed |C:/Users/User/Desktop/FileW/file1.txt | old: 5| new: 15`
- `[12:39:57] status: exists |C:/Users/User/Desktop/FileW/file1.txt | size: 15 bytes`

**Удаление файла test2.txt:**
- `del file2.txt`

**Вывод программы:**
- `[12:40:30] event: deleted |C:/Users/User/Desktop/FileW/file2.txt`
- `[12:40:30] status: no exists |C:/Users/User/Desktop/FileW/file2.txt`

# 4. Тестовые случаи
### Case 1: Мониторинг существующего файла
Условия:
Файл file1.txt существует
Вывод:
- `[18:36:45] status: exists | C:/Users/User/Desktop/FileW/file1.txt | size: 30 bytes`
### Case 2: Изменение размера файла
Условия:
Файл file1.txt существует
Начальный размер 30 байт

Вывод:
- `[18:39:46] event: size changed | C:/Users/User/Desktop/FileW/file1.txt | old: 30 | new: 36`
- `[18:39:46] status: exists | C:/Users/User/Desktop/FileW/file1.txt | size: 36 bytes`
### Case 3: Удаление файла
Условия:
Файл file2.txt существует и отслеживается программой

Вывод:
- `[18:46:00] event: deleted | C:/Users/User/Desktop/FileW/file2.txt`
- `[18:46:00] status: no exists | C:/Users/User/Desktop/FileW/file2.txt`
### Case 4: Мониторинг нескольких файлов одновременно
Условия:
Файлы file1.txt и file2.txt существуют

Вывод:
- `[12:38:03] File watcher`
- `[12:38:03] Check interval: 100 ms`
- `[12:38:03] Watched files:`
- `[12:38:03]   C:/Users/User/Desktop/FileW/file1.txt`
- `[12:38:03]   C:/Users/User/Desktop/FileW/file2.txt`
- `[12:38:03] Initial state:`
- `[12:38:03] status: exists |C:/Users/User/Desktop/FileW/file1.txt | size: 5 bytes`
- `[12:38:03] status: exists |C:/Users/User/Desktop/FileW/file2.txt | size: 4 bytes`
### Case 5: Запуск без указания файлов   
Условия:
Не указаны файлы для наблюдения в командной строке

Вывод:
- `error: no files specified for monitoring!`
### Case 6: Настройка интервала проверки
Условия: Файл test.txt существует

Действия:
Запуск: FileW.exe --interval=1000 C:/Users/User/Desktop/FileW/file1.txt 

Вывод:
- `[20:21:01] File watcher`
- `[20:21:01] Check interval: 1000 ms`
- `[20:21:01] Watched files:`
- `[20:21:01]   C:/Users/User/Desktop/FileW/file1.txt`
- `[20:21:01] Initial state:`
- `[20:21:01] status: exists | C:/Users/User/Desktop/FileW/file1.txt | size: 36 bytes`

### Case 7: Несуществующий путь к файлу
Условия: Путь C:\invalid\path\file.txt не существует

Действия:

Запуск:  - `C:\invalid\file1.txt`

Вывод:
- `[20:25:11] File watcher`
- `[20:25:11] Check interval: 100 ms`
- `[20:25:11] Watched files:`
- `[20:25:11]   C:\invalid\file1.txt`
- `[20:25:11] Initial state:`
- `[20:25:11] status: no exists | C:\invalid\file1.txt`
### Case 8: Модификация файла без изменения размера
Условия:

- Файл file1.txt существует
- Размер: 36 байт

Действия:

Внести изменения и сохранить (размер остался 36 байт)

Вывод программы:
- `[20:27:58] event: modified | C:/Users/User/Desktop/FileW/file1.txt | size: 36 bytes`
- `[20:27:58] status: exists | C:/Users/User/Desktop/FileW/file1.txt | size: 36 bytes`
### Case 9: Добавление дубликата файла
Условия:

Файл file1.txt уже отслеживается программой

Действия:

Попытка добавить тот же файл повторно

Вывод:
- `[20:27:05] status: exists | C:/Users/User/Desktop/FileW/file1.txt | size: 36 bytes `
