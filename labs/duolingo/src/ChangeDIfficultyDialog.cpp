#include "ChangeDIfficultyDialog.h"

#include "Settings.h"
#include "TaskDifficulty.h"

#include <QComboBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

ChangeDIfficultyDialog::ChangeDIfficultyDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Выбрать сложность");

    auto* variants = new QComboBox(this);
    variants->addItem(QString("Легкая"));
    variants->addItem(QString("Средняя"));
    variants->addItem(QString("Сложная"));
    variants->setCurrentIndex(static_cast<int>(Settings::GetTaskDifficulty()));

    auto* layout = new QVBoxLayout;
    layout->addWidget(variants);

    auto* accept_button = new QPushButton(QString("Принять изменения"));
    layout->addWidget(accept_button);

    connect(accept_button, &QPushButton::clicked, this, [this, variants] {
        Settings::SetTaskDifficulty(static_cast<TaskDifficulty>(variants->currentIndex()));
        this->accept();
    });

    setLayout(layout);

    resize(300, 200);
}
