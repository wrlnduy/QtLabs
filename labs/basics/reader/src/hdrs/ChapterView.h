#ifndef CHAPTERVIEW_H
#define CHAPTERVIEW_H

#include <QComboBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class ChapterView : public QWidget {
    Q_OBJECT
   public:
    ChapterView(
        const QString& titleName, const QString& ruTitleName, const int& chapterIndex,
        QWidget* parent = nullptr);

   private slots:
    void goToRanobeView();
    void toNextChapter();
    void toPrevChapter();
    void toChosenChapter();

   signals:
    void toRanobeView(const QString& titleName);

   private:
    QString titleName_{};
    QString chapterFilename_{};
    int chapterIndex_{};
    int chapterCount_{};
    QJsonArray chapterFilenames_{};
    QJsonObject chapterNames_{};
    QPushButton* toRanobeViewButton_{};
    QPushButton* topPrevChapterButton_{};
    QPushButton* topNextChapterButton_{};
    QPushButton* bottomPrevChapterButton_{};
    QPushButton* bottomNextChapterButton_{};
    QLabel* chapterText_{};
    QLabel* chapterName_;
    QComboBox* chapterChooseBox_{};

    QString readTxt(const QString& path);
};

#endif  // CHAPTERVIEW_H
