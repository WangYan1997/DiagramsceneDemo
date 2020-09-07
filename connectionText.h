#ifndef CONNECTIONTEXT_H
#define CONNECTIONTEXT_H

#include <QGraphicsLineItem>
#include "diagramitem.h"
#include "diagramtextitem.h"

#include <QDebug>

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class ConnectionText : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 5 };

    ConnectionText(DiagramTextItem *startItem, DiagramTextItem *endItem, QGraphicsItem *parent = 0);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) { myColor = color; }
    DiagramTextItem *startItem() const {return myStartItem; }
    DiagramTextItem *endItem() const {return myEndItem; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    DiagramTextItem *myStartItem;
    DiagramTextItem *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
};

#endif // CONNECTIONTEXT_H
