#include "hdrs/RanobeView.h"

#include "hdrs/JsonWork.h"

#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <utility>

RanobeView::RanobeView(
    const QString& titleName, const QString& ruTitleName, QIcon icon, QWidget* parent)
    : QWidget(parent), titleName_(titleName), icon_(std::move(icon)) {
    auto* mainLayout = new QHBoxLayout;
    auto* titleLayout = new QVBoxLayout;
    auto* chaptersLayout = new QVBoxLayout;

    iconLabel_ = new QLabel;
    iconLabel_->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(iconLabel_);

    mainLayout->addLayout(titleLayout);

    chapterList_ = new QListWidget;
    chapterList_->setDragEnabled(false);
    chapterList_->setMovement(QListView::Static);
    chapterList_->setSelectionMode(QAbstractItemView::NoSelection);

    auto titleInfo = JsonWork::parseJson(
        QString(":/resources/books/") + QString(titleName_) + QString("/titleInfo.json"));
    const int chaptersCount = titleInfo["chaptersCount"].toInt();
    const QJsonArray chaptersArray = titleInfo["chapters"].toArray();
    for (int i = 0; i < chaptersCount; i++) {
        const QJsonObject chapter = chaptersArray.at(i).toObject();
        const QString filename = chapter["filename"].toString();
        const QString chapterName = chapter["name"].toString();

        auto* chapterNameLabel = new QLabel(chapterName);

        auto* item = new QListWidgetItem;
        chapterList_->addItem(item);
        chapterList_->setItemWidget(item, chapterNameLabel);
    }

    auto* synopsisLabel = new QLabel(QString("О тайтле\n"));
    synopsisLabel->setStyleSheet("font-weight: bold; color: orange");

    auto* synopsis = new QLabel(titleInfo["synopsis"].toString() + QString("\n"));
    synopsis->setWordWrap(true);

    auto* chaptersLabel = new QLabel(QString("Главы\n"));
    chaptersLabel->setStyleSheet("font-weight: bold; color: orange");

    chaptersLayout->addWidget(synopsisLabel);
    chaptersLayout->addWidget(synopsis);
    chaptersLayout->addWidget(chaptersLabel);
    chaptersLayout->addWidget(chapterList_);

    setIconSize(QSize(100, 90));

    mainLayout->addLayout(chaptersLayout);

    setLayout(mainLayout);
}

void RanobeView::setIconSize(const QSize& size) {
    iconLabel_->setPixmap(icon_.pixmap(size));
}