#include "Raycaster.h"

#include "FPSCounter.h"
#include "GraphicsView.h"
#include "Polygon.h"

#include <QBrush>
#include <QColorDialog>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainterPath>
#include <QPen>
#include <QRadioButton>
#include <QShortcut>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <cstddef>

Raycaster::Raycaster(QWidget* parent) : QMainWindow(parent) {
    auto* central = new QWidget;

    setCentralWidget(central);

    auto* layout = new QVBoxLayout;

    auto* top_layout_ = new QHBoxLayout;

    light_mode_ = new QRadioButton("Light");
    polygon_mode_ = new QRadioButton("Polygon");
    static_lights_mode_ = new QRadioButton("Static Lights");

    light_mode_->setChecked(true);

    fps_counter_ = new FPSCounter;

    top_layout_->addWidget(new QLabel("Mode: "));
    top_layout_->addWidget(light_mode_);
    top_layout_->addWidget(polygon_mode_);
    top_layout_->addWidget(static_lights_mode_);
    top_layout_->addStretch(1);
    top_layout_->addWidget(fps_counter_);

    connect(light_mode_, &QRadioButton::toggled, this, &Raycaster::LightModeToggled);
    connect(polygon_mode_, &QRadioButton::toggled, this, &Raycaster::PolygonModeToggled);
    connect(static_lights_mode_, &QRadioButton::toggled, this, &Raycaster::StaticLightsModeToggled);

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
            PolygonModeToggled(true);
        }
    });

    const auto* remove_last_polygon_shortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    connect(remove_last_polygon_shortcut, &QShortcut::activated, this, [this] {
        switch (mode_) {
            case InputModes::Polygon:
                if (controller_.GetPolygons().back().GetVertices().empty()) {
                    controller_.RemoveLastPolygon();
                }
                if (controller_.GetPolygons().size() > 1) {
                    controller_.RemoveLastPolygon();
                }
                if (mode_ == InputModes::Polygon) {
                    PolygonModeToggled(true);
                }
                break;
            case InputModes::StaticLights:
                if (!controller_.GetStaticLights().empty()) {
                    controller_.RemoveLastStaticLight();
                }
                break;
            default:;
        }
    });

    const auto* end_polygon_shortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
    connect(end_polygon_shortcut, &QShortcut::activated, this, [this] {
        MousePressed({}, Qt::RightButton);
    });

    const auto* clear_static_lights_shortcut = new QShortcut(QKeySequence("Alt+R"), this);
    connect(clear_static_lights_shortcut, &QShortcut::activated, this, [this] {
        while (!controller_.GetStaticLights().empty()) {
            controller_.RemoveLastStaticLight();
        }
    });

    refresh_timer_ = new QTimer;
    connect(refresh_timer_, &QTimer::timeout, this, [this] { Render(); });
    refresh_timer_->setTimerType(Qt::PreciseTimer);
    refresh_timer_->start(1);

    layout->addWidget(view_);

    central->setLayout(layout);
}

void Raycaster::LightModeToggled(bool checked) {
    if (!checked) {
        return;
    }
    mode_ = InputModes::Light;
}

void Raycaster::PolygonModeToggled(bool checked) {
    if (!checked) {
        MousePressedPolygon({}, Qt::RightButton);
        controller_.RemoveLastPolygon();
        return;
    }
    mode_ = InputModes::Polygon;
    controller_.AddPolygon({}, PolygonType::Creating);
}

void Raycaster::StaticLightsModeToggled(bool checked) {
    if (!checked) {
        return;
    }
    mode_ = InputModes::StaticLights;
}

void Raycaster::MouseMoved(const QPointF& scene_pos) {
    switch (mode_) {
        case InputModes::Light:
            MouseMovedLight(scene_pos);
        default:;
    }
}

void Raycaster::MousePressed(const QPointF& scene_pos, Qt::MouseButton button) {
    switch (mode_) {
        case InputModes::Polygon:
            MousePressedPolygon(scene_pos, button);
            break;
        case InputModes::StaticLights:
            MousePressedStaticLights(scene_pos, button);
            break;
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
    for (size_t i = 1; i < vertices.size(); ++i) {
        path.lineTo(vertices[i]);
    }
    switch (polygon.GetType()) {
        case PolygonType::Creating:
            path.addEllipse(vertices.back(), 2, 2);
            break;
        case PolygonType::Finished:
            path.closeSubpath();
            break;
        default:;
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
    for (const auto& [light, color] : controller_.GetStaticLights()) {
        path.clear();
        path.addEllipse(light, controller_.GetLightRadius(), controller_.GetLightRadius());
        scene_->addPath(
            path, QPen(Qt::black), QBrush(QColor(color.red(), color.green(), color.blue())));
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
    for (const auto& [light, color] : controller_.GetStaticLights()) {
        const auto& light_area = controller_.CreateLightArea(light);
        DrawPolygon(light_area, QPen(Qt::transparent), QBrush(color));
    }
}

void Raycaster::MouseMovedLight(const QPointF& scene_pos) {
    if (controller_.CanPlaceLight(scene_pos)) {
        controller_.SetLightSource(scene_pos);
    }
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

void Raycaster::MousePressedStaticLights(const QPointF& scene_pos, Qt::MouseButton button) {
    if (button == Qt::RightButton) {
        return;
    }
    if (controller_.StaticLightsOverflow()) {
        QMessageBox::warning(this, ",jkmit yt ,eltn uhecnyj", "Too much static lights.");
        return;
    }
    if (!controller_.CanPlaceLight(scene_pos)) {
        QMessageBox::warning(
            this, ",jkmit yt ,eltn uhecnyj",
            "Can't be placed here (inside of polygon / too close to polygon)");
        return;
    }
    refresh_timer_->stop();
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose light color");
    color.setAlpha(128);
    controller_.AddStaticLight(scene_pos, color);
    refresh_timer_->start();
}
