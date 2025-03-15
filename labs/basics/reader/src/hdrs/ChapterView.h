#ifndef CHAPTERVIEW_H
#define CHAPTERVIEW_H

#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class ChapterView : public QWidget {
    Q_OBJECT
   public:
    ChapterView(const QString& titleName, const int& chapterIndex, QWidget* parent = nullptr);

   private slots:
    void toNextChapter();
    void toPrevChapter();

   signals:
    void toRanobeView(const QString& titleName);

   private:
    QString titleName_{};
    QString chapterFilename_{};
    int chapterIndex_{};
    int chapterCount_{};
    QJsonArray chapterFilenames_{};
    QJsonObject chapterNames_{};
    QPushButton* prevChapterButton_{};
    QPushButton* nextChapterButton_{};
    QLabel* chapterText_{};
    QLabel* chapterName_;

    QString readTxt(const QString& path);
};

#endif  // CHAPTERVIEW_H
