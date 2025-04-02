#include "Raycaster.h"

#include "FPSCounter.h"
#include "GraphicsView.h"
#include "Polygon.h"

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPainterPath>
#include <QPen>
#include <QRadioButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <cstddef>

Raycaster::Raycaster(QWidget* parent) : QMainWindow(parent) {
    auto* central = new QWidget;

    setCentralWidget(central);

    auto* layout = new QVBoxLayout;

    auto* top_layout_ = new QHBoxLayout;

    light_mode_ = new QRadioButton("Light");
    polygon_mode_ = new QRadioButton("Polygon");

    light_mode_->setChecked(true);

    fps_counter_ = new FPSCounter;

    top_layout_->addWidget(new QLabel("Mode: "));
    top_layout_->addWidget(light_mode_);
    top_layout_->addWidget(polygon_mode_);
    top_layout_->addStretch(1);
    top_layout_->addWidget(fps_counter_);

    connect(light_mode_, &QRadioButton::toggled, this, &Raycaster::LightModePressed);
    connect(polygon_mode_, &QRadioButton::toggled, this, &Raycaster::PolygonModePressed);

    layout->addLayout(top_layout_);

    view_ = new GraphicsView;
    view_->setMinimumSize(800, 600);

    scene_ = new QGraphicsScene;
    scene_->setSceneRect(0, 0, 800, 600);

    controller_.AddPolygon(
        Polygon({QPointF(0, 0), QPointF(0, 600), QPointF(800, 600), QPointF(800, 0)}));
    controller_.SetLightSource({400, 300});

    Render();

    view_->setScene(scene_);

    connect(view_, &GraphicsView::MouseMoved, this, &Raycaster::MouseMoved);
    connect(view_, &GraphicsView::MousePressed, this, &Raycaster::MousePressed);

    layout->addWidget(view_);

    central->setLayout(layout);
}

void Raycaster::LightModePressed(bool checked) {
    if (!checked) {
        return;
    }
    mode_ = Modes::Light;
}

void Raycaster::PolygonModePressed(bool checked) {
    if (!checked) {
        MousePressedPolygon({}, Qt::RightButton);
        controller_.RemoveLastPolygon();
        return;
    }
    mode_ = Modes::Polygon;
    controller_.AddPolygon({});
}

void Raycaster::MouseMoved(const QPointF& scene_pos) {
    switch (mode_) {
        case Modes::Light:
            MouseMovedLight(scene_pos);
        case Modes::Polygon:
            break;
        default:;
    }
}

void Raycaster::MousePressed(const QPointF& scene_pos, Qt::MouseButton button) {
    switch (mode_) {
        case Modes::Light:
            break;
        case Modes::Polygon:
            MousePressedPolygon(scene_pos, button);
        default:;
    }
}

void Raycaster::Render() const {
    DrawPolygons();

    const auto light_area = controller_.CreateLightArea();
    DrawPolygon(light_area, QPen(Qt::red), QBrush(QColor(250, 92, 92)));

    DrawLight();
}

void Raycaster::DrawPolygon(const Polygon& polygon, const QPen& pen, const QBrush& brush) const {
    const auto& vertices = polygon.GetVertices();

    if (vertices.empty()) {
        return;
    }

    QPainterPath path;
    path.moveTo(vertices[0]);

    for (size_t i = 1; i < vertices.size(); ++i) {
        path.lineTo(vertices[i]);
    }

    if (vertices.size() > 2) {
        path.lineTo(vertices[0]);
    }

    scene_->addPath(path, pen, brush);
}

void Raycaster::DrawPolygons() const {
    scene_->clear();

    for (const auto& polygon : controller_.GetPolygons()) {
        DrawPolygon(polygon, QPen(Qt::black, 2), QBrush(Qt::gray));
    }
}

void Raycaster::DrawLight() const {
    QPainterPath path;

    path.addEllipse(controller_.GetLightSource(), 3, 3);
    scene_->addPath(path, QPen(Qt::darkRed), QBrush(Qt::darkRed));
}

void Raycaster::MouseMovedLight(const QPointF& scene_pos) {
    controller_.SetLightSource(scene_pos);
    Render();
}

void Raycaster::MousePressedPolygon(const QPointF& scene_pos, Qt::MouseButton button) {
    if (button == Qt::RightButton) {
        if (controller_.GetPolygons().back().GetVertices().empty()) {
            controller_.RemoveLastPolygon();
        } else {
            Render();
        }
        controller_.AddPolygon({});
        return;
    }
    controller_.AddVertexToLastPolygon(scene_pos);
    QPainterPath path;
    const auto& vertices = controller_.GetPolygons().back().GetVertices();
    path.moveTo(vertices.back());
    if (vertices.size() > 1) {
        path.lineTo(vertices[vertices.size() - 2]);
    } else {
        path.addEllipse(vertices.back(), .5, .5);
    }
    scene_->addPath(path, QPen(Qt::black, 2), QBrush(Qt::gray));
}
