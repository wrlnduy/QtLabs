#include "Settings.h"

#include "ExerciseTypes.h"
#include "TaskDifficulty.h"

void Settings::SetTaskDifficulty(const TaskDifficulty& difficulty) {
    settings_.setValue("TaskDifficulty", static_cast<int>(difficulty));
    emit DifficultyChanged(difficulty);
}

TaskDifficulty Settings::GetTaskDifficulty() const {
    const auto& difficulty = settings_.value("TaskDifficulty", 0).toInt();
    return static_cast<TaskDifficulty>(difficulty);
}

void Settings::SetExerciseType(const ExerciseType& exercise) {
    settings_.setValue("ExerciseType", static_cast<int>(exercise));
}

ExerciseType Settings::GetExerciseType() const {
    const auto& exercise = settings_.value("ExerciseType", 0).toInt();
    return static_cast<ExerciseType>(exercise);
}
