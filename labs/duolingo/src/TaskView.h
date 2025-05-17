#ifndef TASKVIEW_H
#define TASKVIEW_H

#include "ExerciseTypes.h"
#include "GrammarView.h"
#include "Settings.h"
#include "TaskDifficulty.h"
#include "TranslationView.h"

#include <QLabel>
#include <QProgressBar>
#include <QStackedWidget>
#include <QTimer>
#include <QWidget>

constexpr int kExerciseDuration = 200 * 1000;
constexpr int kMistakes = 3;
constexpr int kTasksInSet = 5;

class TaskView : public QWidget {
    Q_OBJECT
   public:
    explicit TaskView(const std::shared_ptr<Settings>&, QWidget* parent = nullptr);

    void SetExercise(const ExerciseType&, const TaskDifficulty&);

   private slots:
    void HandleAccepted();
    void HandleWrongAnswer();

   private:
    QStackedWidget* stacked_widget_ = nullptr;
    GrammarView* grammar_view_ = nullptr;
    TranslationView* translation_view_ = nullptr;

    QProgressBar* progress_bar_ = nullptr;
    QTimer* exercise_timer_ = nullptr;
    QTimer* refresh_timer_ = nullptr;
    QLabel* timer_label_ = nullptr;

    ExerciseType exercise_type_;
    TaskDifficulty task_difficulty_;
    std::vector<int> task_ids{};

    std::shared_ptr<Settings> settings_{};

    int mistakes_{};

    void RefreshStats();

    void FindTasks();

    void LoadNext();
};

#endif  // TASKVIEW_H
