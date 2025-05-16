#ifndef SETTINGS_H
#define SETTINGS_H

#include "ExerciseTypes.h"
#include "TaskDifficulty.h"

#include <QObject>
#include <QSettings>

class Settings : public QObject {
    Q_OBJECT
   public:
    void SetTaskDifficulty(const TaskDifficulty&);
    [[nodiscard]] TaskDifficulty GetTaskDifficulty() const;

    void SetExerciseType(const ExerciseType&);
    [[nodiscard]] ExerciseType GetExerciseType() const;

   signals:
    void DifficultyChanged(TaskDifficulty difficulty) const;

   private:
    QSettings settings_{};
};

#endif  // SETTINGS_H
