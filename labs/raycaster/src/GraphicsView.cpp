#include "GraphicsView.h"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWidget>

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {
    setMouseTracking(true);
}

void GraphicsView::mousePressEvent(QMouseEvent* event) {
    QGraphicsView::mousePressEvent(event);
    emit MousePressed(mapToScene(event->pos()), event->button());
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event) {
    QGraphicsView::mouseMoveEvent(event);
    emit MouseMoved(mapToScene(event->pos()));
}
