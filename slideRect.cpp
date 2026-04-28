#include "stdafx.h"
#include "slideRect.h"

slideRect::slideRect(QGraphicsView* parent,double scale,QSize size)
	: QGraphicsRectItem(), view(parent), scale(scale), m_dragging(false), m_size(size)
{
	setBrush(QBrush(QColor(100, 150, 200, 128)));
	setPen(QPen(QColor(50, 100, 150, 200), 2));
	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsSelectable, false);  // 不选中，避免出现焦点框
	setFlag(QGraphicsItem::ItemIsMovable, false);     // 禁用内置移动，手动实现垂直拖动

	setPos(0, 0);
	setZValue(1000);
	updatePosition();
}

slideRect::~slideRect()
{}

void slideRect::updatePosition()
{
	if (!view) return;
	qreal sceneWidth = m_size.width();
	qreal rectHeight = scale * sceneWidth;

	setRect(0, 0, sceneWidth, rectHeight);
	qreal newY = getRestrictedY(pos().y());
	setPos(0, newY);
}

void slideRect::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		m_dragging = true;
		m_dragStartScene = event->scenePos();
		m_itemStartPos = pos();
		event->accept();
	}
	else {
		QGraphicsRectItem::mousePressEvent(event);
	}
}
void slideRect::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_dragging) {
		QPointF delta = event->scenePos() - m_dragStartScene;
		qreal newY = getRestrictedY(m_itemStartPos.y() + delta.y());
		setPos(0, newY);
		event->accept();
	}
	else {
		QGraphicsRectItem::mouseMoveEvent(event);
	}
}
void slideRect::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_dragging && event->button() == Qt::LeftButton) {
		m_dragging = false;
		event->accept();
	}
	else {
		QGraphicsRectItem::mouseReleaseEvent(event);
	}
}

qreal slideRect::getRestrictedY(qreal y) const
{
	if (!view) return y;
	qreal sceneHeight = m_size.height();
	qreal rectHeight = scale * m_size.width();
	qreal minY = 0;
	qreal maxY = sceneHeight - rectHeight;
	return qBound(minY, y, maxY);
}