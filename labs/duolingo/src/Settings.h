#ifndef SETTINGS_H
#define SETTINGS_H

#include "ExerciseTypes.h"
#include "TaskDifficulty.h"

#include <QSettings>

namespace Settings {
inline QSettings settings{};

void SetTaskDifficulty(const TaskDifficulty&);
[[nodiscard]] TaskDifficulty GetTaskDifficulty();

void SetExerciseType(const ExerciseType&);
[[nodiscard]] ExerciseType GetExerciseType();

};  // namespace Settings

#endif  // SETTINGS_H
