#include "TaskView.h"

#include "DataBase.h"
#include "ExerciseTypes.h"
#include "GrammarView.h"
#include "TranslationView.h"

#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QStackedWidget>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

TaskView::TaskView(const std::shared_ptr<Settings>& settings, QWidget* parent)
    : QWidget(parent)
    , stacked_widget_(new QStackedWidget(this))
    , grammar_view_(new GrammarView(this))
    , translation_view_(new TranslationView(this))
    , progress_bar_(new QProgressBar(this))
    , exercise_timer_(new QTimer(this))
    , refresh_timer_(new QTimer(this))
    , timer_label_(new QLabel(QString("Осталось времени:...с"), this)) {
    settings_ = settings;
    stacked_widget_->addWidget(grammar_view_);
    stacked_widget_->addWidget(translation_view_);

    auto* main_layout = new QVBoxLayout;

    auto* info_layout = new QHBoxLayout;
    info_layout->addWidget(progress_bar_);
    info_layout->addWidget(timer_label_);
    exercise_timer_->setTimerType(Qt::PreciseTimer);
    refresh_timer_->setTimerType(Qt::PreciseTimer);
    exercise_timer_->setInterval(kExerciseDuration);
    refresh_timer_->setInterval(10);
    connect(refresh_timer_, &QTimer::timeout, this, [this] {
        if (!exercise_timer_->isActive()) {
            timer_label_->setText(QString("Осталось времени:...с"));
            return;
        }
        const int time = exercise_timer_->remainingTime();
        const int sec = time / 1000;
        const int msec = (time - (sec * 1000)) / 10;
        timer_label_->setText(QString("Осталось времени:%1,%2с").arg(sec).arg(msec));
    });
    refresh_timer_->start();

    RefreshStats();

    main_layout->addLayout(info_layout);
    main_layout->addWidget(stacked_widget_);
    setLayout(main_layout);

    connect(grammar_view_, &GrammarView::Accepted, this, &TaskView::HandleAccepted);
    connect(grammar_view_, &GrammarView::WrongAnswer, this, &TaskView::HandleWrongAnswer);

    // connect(translation_view_, &TranslationView::Accepted, this, &TaskView::HandleAccepted);
    // connect(translation_view_, &TranslationView::WrongAnswer, this, &TaskView::HandleWrongAnswer);
}

void TaskView::HandleAccepted() {
    progress_bar_->setValue(progress_bar_->value() + 1);
    LoadNext();
}

void TaskView::HandleWrongAnswer() {
    ++mistakes_;
    if (mistakes_ == kMistakes) {
    }
    LoadNext();
}

void TaskView::SetExercise(const ExerciseType& exercise, const TaskDifficulty& difficulty) {
    // FindTasks();
    if (exercise == ExerciseType::Grammar) {
        stacked_widget_->setCurrentWidget(grammar_view_);
        // grammar_view_->SetTask(task_difficulty_, task_ids.back());
    } else {
        stacked_widget_->setCurrentWidget(translation_view_);
    }
    RefreshStats();
    exercise_timer_->start();
    exercise_type_ = exercise;
    task_difficulty_ = difficulty;
}

void TaskView::FindTasks() {
    task_ids = settings_->GetUnusedTasks(
        static_cast<int>(exercise_type_), static_cast<int>(task_difficulty_));
}

void TaskView::RefreshStats() {
    progress_bar_->setValue(0);
    progress_bar_->setMaximum(kTasksInSet);

    mistakes_ = 0;
}

void TaskView::LoadNext() {
    if (task_ids.empty()) {
        QMessageBox::information(this, QString("Победа"), QString("Задание выполнено!"));
        FindTasks();
    }
    int next = task_ids.back();
    task_ids.pop_back();
    if (exercise_type_ == ExerciseType::Grammar) {
        grammar_view_->SetTask(task_difficulty_, next);
    } else {
        // translation_view_->SetTask(task_difficulty_, next);
    }
    exercise_timer_->start();
}