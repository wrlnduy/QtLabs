#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "Controller.h"
#include "FPSCounter.h"
#include "GraphicsView.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>

enum class Modes { Light, Polygon };

class Raycaster final : public QMainWindow {
    Q_OBJECT
   public:
    Raycaster(QWidget* parent = nullptr);
   private slots:
    void LightModePressed(bool checked);
    void PolygonModePressed(bool checked);
    void MouseMoved(const QPointF&);
    void MousePressed(const QPointF&, Qt::MouseButton);

   private:
    QRadioButton* light_mode_{};
    QRadioButton* polygon_mode_{};

    FPSCounter* fps_counter_{};

    GraphicsView* view_{};
    QGraphicsScene* scene_{};

    Controller controller_{};
    Modes mode_ = Modes::Light;

    void Render() const;
    void DrawPolygon(const Polygon&, const QPen&, const QBrush&) const;
    void DrawPolygons() const;
    void DrawLight() const;
    void MouseMovedLight(const QPointF&);
    void MousePressedPolygon(const QPointF&, Qt::MouseButton);
};

#endif