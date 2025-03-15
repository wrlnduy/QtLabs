#ifndef RANOBE_READER_H
#define RANOBE_READER_H

#include "JsonWork.h"
#include "RanobeList.h"
#include "RanobeView.h"

#include <QJsonObject>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QResizeEvent>
#include <QString>
#include <QStackedWidget>

enum class LayoutType { EmptyLayout, RanobeList, RanobeView };

class RanobeReader : public QMainWindow {
    Q_OBJECT
   public:
    RanobeReader(QWidget* parent = nullptr);

    void clearWindow() const;

    void setWindowLayout(QLayout* layout) const;

    void setRanobeListLayout();

    void setRanobeViewLayout(const QString& titleName);

   private slots:
    void RanobeChosen(QListWidgetItem* ranobe);

   protected:
    void resizeEvent(QResizeEvent* event) override;

   private:
    QWidget* central_{};
    QStackedWidget* widgetStack_{};
    RanobeList* ranobeList_{};
    RanobeView* ranobeView_{};

    LayoutType currentLayout = LayoutType::EmptyLayout;
    QJsonObject cfg_{};
};

#endif  // RANOBE_READER_H
