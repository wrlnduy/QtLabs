#include "Settings.h"

#include "ExerciseTypes.h"
#include "TaskDifficulty.h"

void Settings::SetTaskDifficulty(const TaskDifficulty& difficulty) {
    settings.setValue("TaskDifficulty", static_cast<int>(difficulty));
}

TaskDifficulty Settings::GetTaskDifficulty() {
    const auto& difficulty = settings.value("TaskDifficulty", 0).toInt();
    return static_cast<TaskDifficulty>(difficulty);
}

void Settings::SetExerciseType(const ExerciseType& exercise) {
    settings.setValue("ExerciseType", static_cast<int>(exercise));
}


ExerciseType Settings::GetExerciseType() {
    const auto& exercise = settings.value("ExerciseType", 0).toInt();
    return static_cast<ExerciseType>(exercise);
}
