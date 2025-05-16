#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Settings.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <memory>

class MainWindow : public QMainWindow {
    Q_OBJECT
   public:
    explicit MainWindow(QWidget* parent = nullptr);

   private slots:
    void ShowHelpMessage();

   private:
    std::shared_ptr<Settings> settings_{};
};

#endif  // MAINWINDOW_H
