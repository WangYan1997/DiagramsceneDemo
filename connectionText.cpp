#include "connectionText.h"

#include <qmath.h>
#include <QPen>
#include <QPainter>

ConnectionText::ConnectionText(DiagramTextItem *startItem, DiagramTextItem *endItem, QGraphicsItem *parent)
    :QGraphicsLineItem(parent)
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF ConnectionText::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath ConnectionText::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void ConnectionText::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}


void ConnectionText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qDebug() << "运行到Connection中的paint开头啦！";

    if(myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(myColor);


//   获得text框的中心点
    QPointF startpoint = QPointF(myStartItem->pos().x() + myStartItem->boundingRect().width()/2,
                                 myStartItem->pos().y() + myStartItem->boundingRect().height()/2);
    QPointF endpoint = QPointF(myEndItem->pos().x() + myEndItem->boundingRect().width()/2,
                                 myEndItem->pos().y() + myEndItem->boundingRect().height()/2);

//  centerLine应为text框的中心点之间的连线
    QLineF centerLine(startpoint, endpoint);
    QPolygonF endPolygon = QPolygonF(myEndItem->boundingRect());
    QPolygonF startPolygon = QPolygonF(myStartItem->boundingRect());
    QPointF endp1 = endPolygon.first() + myEndItem->pos();
    QPointF startp1 = startPolygon.first() + myStartItem->pos();
    QPointF endp2;
    QPointF startp2;
    QPointF intersectEndPoint;
    QPointF intersectStartPoint;
    QLineF polyLine;

//    计算intersectPoint，即centerLine（text框的中心点之间的连线）和 text框 的相交点
    for (int i = 1; i < endPolygon.count(); ++i) {
        endp2 = endPolygon.at(i) + myEndItem->pos();
        polyLine = QLineF(endp1, endp2);
        QLineF::IntersectType intersectType =
            polyLine.intersect(centerLine, &intersectEndPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        endp1 = endp2;
    }

    for (int i = 1; i < startPolygon.count(); ++i) {
        startp2 = startPolygon.at(i) + myStartItem->pos();
        polyLine = QLineF(startp1, startp2);
        QLineF::IntersectType intersectType =
            polyLine.intersect(centerLine, &intersectStartPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        startp1 = startp2;
    }

    setLine(QLineF(intersectEndPoint, intersectStartPoint));

//    绘制箭头
    double angle = std::atan2(-line().dy(), line().dx());

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                    cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                    cos(angle + M_PI - M_PI / 3) * arrowSize);

    arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;

//  在painter上绘制直线和箭头
    painter->drawLine(line());
    painter->drawPolygon(arrowHead);

    if(isSelected()){
//        选中之后的虚线框
        painter->setPen(QPen(myColor,1,Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }


}
