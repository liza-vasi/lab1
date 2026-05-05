#include <QCoreApplication>
#include <QStringList>
#include <thread>
#include <chrono>
#include <atomic>
#include <csignal>
#include <iostream>
#include "filemanager.h"
#include "logger.h"

std::atomic<bool> running(true);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QStringList args = app.arguments();
    QStringList filePaths;
    int intervalMs = 100;

    for (int i = 1; i < args.size(); ++i) {
        QString arg = args[i];
        if (arg.startsWith("--interval=")) {
            intervalMs = arg.mid(11).toInt();
        } else {
            filePaths.append(arg);
        }
    }

    if (filePaths.isEmpty()) {
        std::cout << "error: no files specified for monitoring!" << std::endl;
        return 1;
    }

    FileManager& manager = FileManager::instance();
    Logger& logger = Logger::instance();

    QObject::connect(&manager, &FileManager::fileCreate, &logger, &Logger::onFileCreated);
    QObject::connect(&manager, &FileManager::fileDelete, &logger, &Logger::onFileDeleted);
    QObject::connect(&manager, &FileManager::fileChangeSize, &logger, &Logger::onFileSizeChanged);
    QObject::connect(&manager, &FileManager::fileModif, &logger, &Logger::onFileModified);
    QObject::connect(&manager, &FileManager::fileChange, &logger, &Logger::onFileChanged);

    logger.log("File watcher");
    logger.log("Check interval: " + QString::number(intervalMs) + " ms");
    logger.log("Watched files:");
    for (const QString &path : filePaths) {
        logger.log("  " + path);
    }
    logger.log("Initial state:");

    manager.addFiles(filePaths);
    while (running) {
        manager.checkAllFiles();


        std::string command;
        std::getline(std::cin, command);

        if (command == "exit") {
            running = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
    logger.log("Program terminated");
    return 0;
}
