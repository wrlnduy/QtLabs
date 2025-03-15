#include "hdrs/ChapterView.h"

#include "hdrs/JsonWork.h"

#include <QScrollArea>
#include <QVBoxLayout>

ChapterView::ChapterView(
    const QString& titleName, const QString& ruTitleName, const int& chapterIndex, QWidget* parent)
    : QWidget(parent), titleName_(titleName), chapterIndex_(chapterIndex) {
    auto titleInfo = JsonWork::parseJson(
        QString(":/resources/books/") + QString(titleName_) + QString("/titleInfo.json"));
    chapterCount_ = titleInfo["chapterCount"].toInt();
    chapterFilenames_ = titleInfo["chapterFilenames"].toArray();
    chapterNames_ = titleInfo["chapterNames"].toObject();
    chapterFilename_ = chapterFilenames_.at(chapterIndex_).toString();

    auto* layout = new QVBoxLayout;

    auto* topLayout = new QHBoxLayout;

    toRanobeViewButton_ = new QPushButton(QString(ruTitleName));
    topLayout->addWidget(toRanobeViewButton_);

    topLayout->addSpacing(10);

    topPrevChapterButton_ = new QPushButton(QString("<-"));
    topLayout->addWidget(topPrevChapterButton_);

    chapterChooseBox_ = new QComboBox;
    for (int i = 0; i < chapterCount_; i++) {
        chapterChooseBox_->addItem(chapterNames_[chapterFilenames_.at(i).toString()].toString());
    }
    chapterChooseBox_->setCurrentIndex(chapterIndex_);
    topLayout->addWidget(chapterChooseBox_);

    topNextChapterButton_ = new QPushButton(QString("->"));
    topLayout->addWidget(topNextChapterButton_);

    topLayout->addStretch(1);

    layout->addLayout(topLayout);

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

    bottomPrevChapterButton_ = new QPushButton(QString("Назад"));
    if (chapterIndex_ == 0) {
        bottomPrevChapterButton_->setText(QString("Это первая глава"));
    }
    bottomLayout->addWidget(bottomPrevChapterButton_);

    bottomLayout->addStretch(1);

    bottomNextChapterButton_ = new QPushButton(QString("Вперёд"));
    if (chapterIndex_ == chapterCount_ - 1) {
        bottomNextChapterButton_->setText(QString("К тайтлу"));
    }
    bottomLayout->addWidget(bottomNextChapterButton_);

    layout->addLayout(bottomLayout);

    connect(toRanobeViewButton_, &QPushButton::pressed, this, &ChapterView::goToRanobeView);
    connect(topPrevChapterButton_, &QPushButton::pressed, this, &ChapterView::toPrevChapter);
    connect(chapterChooseBox_, &QComboBox::currentIndexChanged, this, &ChapterView::toChosenChapter);
    connect(topNextChapterButton_, &QPushButton::pressed, this, &ChapterView::toNextChapter);
    connect(bottomPrevChapterButton_, &QPushButton::pressed, this, &ChapterView::toPrevChapter);
    connect(bottomNextChapterButton_, &QPushButton::pressed, this, &ChapterView::toNextChapter);

    setLayout(layout);
}

void ChapterView::goToRanobeView() {
    emit toRanobeView(titleName_);
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
        bottomPrevChapterButton_->setText(QString("Это первая глава"));
    } else {
        bottomPrevChapterButton_->setText(QString("Назад"));
    }
    if (chapterIndex_ != chapterCount_ - 1) {
        bottomNextChapterButton_->setText(QString("Вперёд"));
    } else {
        bottomNextChapterButton_->setText(QString("К тайтлу"));
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
        bottomPrevChapterButton_->setText(QString("Назад"));
    } else {
        bottomPrevChapterButton_->setText(QString("Это первая глава"));
    }
    if (chapterIndex_ == chapterCount_ - 1) {
        bottomNextChapterButton_->setText(QString("К тайтлу"));
    } else {
        bottomNextChapterButton_->setText(QString("Вперёд"));
    }
}

void ChapterView::toChosenChapter() {
    chapterIndex_ = chapterChooseBox_->currentIndex() - 1;
    toNextChapter();
}

QString ChapterView::readTxt(const QString& path) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);
    return std::move(textStream.readAll());
}