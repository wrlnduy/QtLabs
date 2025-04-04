#include "Raycaster.h"

#include "FPSCounter.h"
#include "GraphicsView.h"
#include "Polygon.h"

#include <QBrush>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QMainWindow>
#include <QPainterPath>
#include <QPen>
#include <QRadioButton>
#include <QShortcut>
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
    connect(view_, &GraphicsView::ViewScaled, this, [this](const QPointF& scale) {
        controller_.Scale(scale);
    });

    const auto* clear_view_shortcut = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(clear_view_shortcut, &QShortcut::activated, this, [this] {
        while (controller_.GetPolygons().size() > 1) {
            controller_.RemoveLastPolygon();
        }
        if (mode_ == InputModes::Polygon) {
            PolygonModePressed(true);
        }
    });

    const auto* remove_last_polygon_shortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    connect(remove_last_polygon_shortcut, &QShortcut::activated, this, [this] {
        if (controller_.GetPolygons().back().GetVertices().empty()) {
            controller_.RemoveLastPolygon();
        }
        if (controller_.GetPolygons().size() > 1) {
            controller_.RemoveLastPolygon();
        }
        if (mode_ == InputModes::Polygon) {
            PolygonModePressed(true);
        }
    });

    const auto* end_polygon_shortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
    connect(end_polygon_shortcut, &QShortcut::activated, this, [this] {
        MousePressed({}, Qt::RightButton);
    });

    auto* refresh_timer = new QTimer;
    connect(refresh_timer, &QTimer::timeout, this, [this] { Render(); });
    refresh_timer->setTimerType(Qt::PreciseTimer);
    refresh_timer->start(1);

    layout->addWidget(view_);

    central->setLayout(layout);
}

void Raycaster::LightModePressed(bool checked) {
    if (!checked) {
        return;
    }
    mode_ = InputModes::Light;
}

void Raycaster::PolygonModePressed(bool checked) {
    if (!checked) {
        MousePressedPolygon({}, Qt::RightButton);
        controller_.RemoveLastPolygon();
        return;
    }
    mode_ = InputModes::Polygon;
    controller_.AddPolygon({}, PolygonType::Creating);
}

void Raycaster::MouseMoved(const QPointF& scene_pos) {
    switch (mode_) {
        case InputModes::Light:
            MouseMovedLight(scene_pos);
        case InputModes::Polygon:
            break;
        default:;
    }
}

void Raycaster::MousePressed(const QPointF& scene_pos, Qt::MouseButton button) {
    switch (mode_) {
        case InputModes::Light:
            break;
        case InputModes::Polygon:
            MousePressedPolygon(scene_pos, button);
        default:;
    }
}

void Raycaster::Render() const {
    DrawPolygons();
    DrawLightArea();
    DrawLight();
}

void Raycaster::DrawPolygon(const Polygon& polygon, const QPen& pen, const QBrush& brush) const {
    const auto& vertices = polygon.GetVertices();
    if (vertices.empty()) {
        return;
    }
    QPainterPath path;
    path.moveTo(vertices[0]);
    if (vertices.size() == 1) {
        path.addEllipse(vertices[0], 2, 2);
    } else {
        for (size_t i = 1; i < vertices.size(); ++i) {
            path.lineTo(vertices[i]);
        }
        if (vertices.size() > 2 && polygon.GetType() == PolygonType::Finished) {
            path.lineTo(vertices[0]);
        }
    }
    scene_->addPath(path, pen, brush);
}

void Raycaster::DrawPolygons() const {
    scene_->clear();

    const auto& polygons = controller_.GetPolygons();

    DrawPolygon(polygons[0], QPen(Qt::transparent), QBrush(Qt::black));
    for (size_t i = 1; i < polygons.size(); i++) {
        DrawPolygon(polygons[i], QPen(Qt::gray), QBrush(Qt::black));
    }
}

void Raycaster::DrawLight() const {
    QPainterPath path;
    for (const auto& delta_light : controller_.GetDeltaLights()) {
        path.clear();
        const auto light = delta_light + controller_.GetLightSource();
        path.addEllipse(light, controller_.GetLightRadius(), controller_.GetLightRadius());
        scene_->addPath(path, QPen(Qt::darkRed), QBrush(Qt::darkRed));
    }
}

void Raycaster::DrawLightArea() const {
    if (mode_ == InputModes::Polygon) {
        return;
    }
    for (const auto& delta_light : controller_.GetDeltaLights()) {
        const auto light = delta_light + controller_.GetLightSource();
        const auto light_area = controller_.CreateLightArea(light);
        DrawPolygon(light_area, QPen(Qt::transparent), QBrush(QColor(75, 82, 82, 128)));
    }
}

void Raycaster::MouseMovedLight(const QPointF& scene_pos) {
    constexpr double kMaxDist = 1;
    const auto& polygons = controller_.GetPolygons();
    for (const auto& delta_light : controller_.GetDeltaLights()) {
        const auto light = delta_light + scene_pos;
        if (controller_.IsTooClose(light, kMaxDist) || !polygons[0].ContainsPoint(light)) {
            return;
        }

        for (size_t i = 1; i < polygons.size(); i++) {
            if (polygons[i].ContainsPoint(light)) {
                return;
            }
        }
    }
    controller_.SetLightSource(scene_pos);
}

void Raycaster::MousePressedPolygon(const QPointF& scene_pos, Qt::MouseButton button) {
    if (button == Qt::RightButton) {
        controller_.SetLastPolygonType(PolygonType::Finished);
        if (controller_.GetPolygons().back().GetVertices().empty()) {
            controller_.RemoveLastPolygon();
        }
        controller_.AddPolygon({}, PolygonType::Creating);
        return;
    }
    controller_.AddVertexToLastPolygon(scene_pos);
}
