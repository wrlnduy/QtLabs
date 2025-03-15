#ifndef RANOBEVIEW_H
#define RANOBEVIEW_H

#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QWidget>

class RanobeView : public QWidget {
    Q_OBJECT
   public:
    RanobeView(
        const QString& titleName, const QString& ruTitleName, QIcon icon,
        QWidget* parent = nullptr);

    void setIconSize(const QSize& size);

   private:
    QString titleName_{};
    QIcon icon_{};
    QLabel* iconLabel_{};
    QListWidget* chapterList_{};
};

#endif  // RANOBEVIEW_H
