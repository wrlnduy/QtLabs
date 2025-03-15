#include "hdrs/ChapterView.h"

#include "hdrs/JsonWork.h"

#include <QScrollArea>
#include <QVBoxLayout>

ChapterView::ChapterView(const QString& titleName, const int& chapterIndex, QWidget* parent)
    : QWidget(parent), titleName_(titleName), chapterIndex_(chapterIndex) {
    auto titleInfo = JsonWork::parseJson(
        QString(":/resources/books/") + QString(titleName_) + QString("/titleInfo.json"));
    chapterCount_ = titleInfo["chapterCount"].toInt();
    chapterFilenames_ = titleInfo["chapterFilenames"].toArray();
    chapterNames_ = titleInfo["chapterNames"].toObject();
    chapterFilename_ = chapterFilenames_.at(chapterIndex_).toString();

    auto* layout = new QVBoxLayout;

    chapterName_ = new QLabel(chapterNames_[chapterFilename_].toString());
    layout->addWidget(chapterName_);

    chapterText_ = new QLabel(readTxt(
        QString(":/resources/books/") + titleName_ + QString("/chapters/") + chapterFilename_ +
        QString(".txt")));
    chapterText_->setWordWrap(true);

    auto* chapterTextScroll = new QScrollArea;
    chapterTextScroll->setWidget(chapterText_);
    chapterTextScroll->setWidgetResizable(true);

    layout->addWidget(chapterTextScroll);

    auto* bottomLayout = new QHBoxLayout;

    prevChapterButton_ = new QPushButton(QString("Назад"));
    if (chapterIndex_ == 0) {
        prevChapterButton_->setText(QString("Это первая глава"));
    }
    bottomLayout->addWidget(prevChapterButton_);

    bottomLayout->addStretch(1);

    nextChapterButton_ = new QPushButton(QString("Вперёд"));
    if (chapterIndex_ == chapterCount_ - 1) {
        nextChapterButton_->setText(QString("К тайтлу"));
    }
    bottomLayout->addWidget(nextChapterButton_);

    layout->addLayout(bottomLayout);

    connect(prevChapterButton_, &QPushButton::pressed, this, &ChapterView::toPrevChapter);
    connect(nextChapterButton_, &QPushButton::pressed, this, &ChapterView::toNextChapter);

    setLayout(layout);
}

void ChapterView::toPrevChapter() {
    if (chapterIndex_ == 0) {
        return;
    }
    chapterIndex_--;
    chapterFilename_ = chapterFilenames_.at(chapterIndex_).toString();
    chapterName_->setText(chapterNames_[chapterFilename_].toString());
    chapterText_->setText(readTxt(
        QString(":/resources/books/") + titleName_ + QString("/chapters/") + chapterFilename_ +
        QString(".txt")));
    if (chapterIndex_ == 0) {
        prevChapterButton_->setText(QString("Это первая глава"));
    }
    if (chapterIndex_ != chapterCount_ - 1) {
        nextChapterButton_->setText(QString("Вперёд"));
    }
}

void ChapterView::toNextChapter() {
    if (chapterIndex_ == chapterCount_ - 1) {
        emit toRanobeView(titleName_);
        return;
    }
    chapterIndex_++;
    chapterFilename_ = chapterFilenames_.at(chapterIndex_).toString();
    chapterName_->setText(chapterNames_[chapterFilename_].toString());
    chapterText_->setText(readTxt(
        QString(":/resources/books/") + titleName_ + QString("/chapters/") + chapterFilename_ +
        QString(".txt")));
    if (chapterIndex_ != 0) {
        prevChapterButton_->setText(QString("Назад"));
    }
    if (chapterIndex_ == chapterCount_ - 1) {
        nextChapterButton_->setText(QString("К тайтлу"));
    }
}

QString ChapterView::readTxt(const QString& path) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);
    return std::move(textStream.readAll());
}