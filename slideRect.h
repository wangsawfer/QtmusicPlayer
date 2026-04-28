#pragma once

#include <QGraphicsRectItem>

class slideRect  : public QGraphicsRectItem
{

public:
	slideRect(QGraphicsView* parent = nullptr, double scale = 1.0,QSize size = QSize());
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	qreal getRestrictedY(qreal y) const;
	~slideRect();
public:
	void updatePosition();
public:
	QGraphicsView* view;
	double scale;
	bool m_dragging;
	QPointF m_dragStartScene;
	QPointF m_itemStartPos;
	QSize m_size;
};

