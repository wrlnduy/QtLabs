#include "MainWindow.h"

#include "ChangeDIfficultyDialog.h"
#include "Settings.h"

#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QShortcut>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), settings_(new Settings) {
    auto* central = new QWidget;
    setCentralWidget(central);
    setWindowTitle("Дуолинго");

    settings_->SetExerciseType(ExerciseType::NoExercise);

    auto* main_layout = new QVBoxLayout;

    auto* menu_bar = new QMenuBar;

    auto* settings_menu = new QMenu(QString("Настройки"));
    const auto* change_difficulty_action = settings_menu->addAction(QString("Выбрать сложность"));
    connect(change_difficulty_action, &QAction::triggered, this, [this]() {
        ChangeDIfficultyDialog(settings_, this).exec();
    });

    auto* help_menu = new QMenu(QString("Помощь"));
    const auto* help_action = help_menu->addAction(QString("Помощь"));
    connect(help_action, &QAction::triggered, this, &MainWindow::ShowHelpMessage);

    const auto* help_shortcut = new QShortcut(QKeySequence("Ctrl+H"), this);
    connect(help_shortcut, &QShortcut::activated, this, &MainWindow::ShowHelpMessage);

    menu_bar->addMenu(settings_menu);
    menu_bar->addMenu(help_menu);
    main_layout->setMenuBar(menu_bar);

    central->setLayout(main_layout);
}

void MainWindow::ShowHelpMessage() {
    QMessageBox::information(
        this, QString("Помощь"),
        [this] -> QString {
            ExerciseType exercise = settings_->GetExerciseType();
            switch (exercise) {
                case ExerciseType::NoExercise:
                    return QString("Упражнение не выбрано");
                default:;
            }
            return {};
        }(),
        QMessageBox::Ok);
}
