#include "freeDrawSelect.h"

#include <QPainter>
#include <QBuffer>
#include <QGraphicsScene>
#include <QJsonArray>

#include "label.h"

/*!
  \class FreeDrawSelect
  \ingroup Selector
  \inmodule InstaDam
  \inherits SelectItem QAbstractGraphicsShapeItem
  \brief The FreeDrawSelect class provides a class for annotating regions by
         painting them.

  Provides a class for creating selection regions defined by painting on the
  canvas with a brush. The region is defined by the pixels that are painted on.
  */


QString FreeDrawSelect::baseInstruction = QString("");

/*!
  Constructs a FreeDwarSelect object based on the given \a map, Label \a label
  and parent \a item, if any.
  */
FreeDrawSelect::FreeDrawSelect(const QPixmap map, QSharedPointer<Label> label,
                               QGraphicsItem *item)
    : QGraphicsPixmapItem(item), SelectItem(label, item) {
    init(label);
    myPen.setWidth(1);
    loadFromPixmap(map);
    setup();
}

/*!
  Constructs a FreeDwarSelect object based on the given \a map and \a pen.
  */
FreeDrawSelect::FreeDrawSelect(const QPixmap map, QPen pen)
    : QGraphicsPixmapItem(nullptr), SelectItem(nullptr, nullptr) {
    init();
    myPen = pen;
    myPen.setWidth(1);
    loadFromPixmap(map);
    setup();
}

/*!
  Constructs a FreeDrawSelect object with no selected points, and a square brush
  with a size of 2 pixels.
  */
FreeDrawSelect::FreeDrawSelect() : FreeDrawSelect(QPointF(-1., -1.), 2,
                                                  Qt::SquareCap) {
}

/*!
  Constructs a FreeDrawSelect object by reading a QJsonObject and setting the
  internal pixel map to the values given in \a json. \a label is the Label which
  owns this object and \a item is the parent QGraphicsItem, if any.
  */
FreeDrawSelect::FreeDrawSelect(const QJsonObject &json,
                               QSharedPointer<Label> label, QGraphicsItem *item)
    : QGraphicsPixmapItem(item), SelectItem(label, item) {
    init(label);
    read(json);
    myPen.setWidth(1);
    setup();
}

/*!
  Constructor used to combine several FreeDrawSelect items given as \a items,
  into a single instance. This is done by merging all of the internal pixel maps
  into a single entity, discarding duplicate points.
  */
FreeDrawSelect::FreeDrawSelect(const QList<FreeDrawSelect*> &items)
    : QGraphicsPixmapItem(nullptr), SelectItem(0.) {
    init();
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myPen.setWidth(1);
    myPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    QListIterator<FreeDrawSelect*> it(items);
    while (it.hasNext()) {
        myPainter.drawPixmap(QPointF(0., 0.), (it.next()->getPixmap()));
    }
    setup();
}

/*!
  Constructs a FreeDrawSelect object with an initial selected point \a point,
  with a brush size of \a brushSize pixels, brush type of \a brushMode, \a label
  as the Label which owns this object and \a item is the parent QGraphicsItem,
  if any. Acceptable values for the \a brushMode are:

  \list
  \li Qt::SquareCap
  \li Qt::RoundCap
  \endlist
  */
FreeDrawSelect::FreeDrawSelect(QPointF point, int brushSize,
                               Qt::PenCapStyle brushMode,
                               QSharedPointer<Label> label,
                               QGraphicsItem *item)
    : QGraphicsPixmapItem(item), SelectItem(label, item){
    init(label);
    myPen.setWidth(brushSize);
    myPen.setCapStyle(brushMode);
    myPen.setJoinStyle(Qt::MiterJoin);
    myPen.setMiterLimit(0.1);
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myPainter.drawPoint(point);
    myPainter.end();
    setup();
}

/*!
  Constructs a FreeDrawSelect object with an initial selected point \a point,
  with a QPen \a pen, \a label
  as the Label which owns this object and \a item is the parent QGraphicsItem,
  if any.
  */
FreeDrawSelect::FreeDrawSelect(QPointF point, QPen pen,
                               QSharedPointer<Label> label, QGraphicsItem *item)
    : FreeDrawSelect(point, pen.width(), pen.capStyle(), label, item) {
}

/*!
  Destructor
  */
FreeDrawSelect::~FreeDrawSelect() {
}

/*---------------------------- Overrides ------------------------*/
/*!
  \reimp
  */
void FreeDrawSelect::addPoint(QPointF &point, const int vertex) {
    UNUSED(point);
    UNUSED(vertex);
}

/*!
  \reimp
  */
QRectF FreeDrawSelect::boundingRect() const {
    return myRect;
}

/*!
  \reimp
  */
void FreeDrawSelect::clickPoint(const QPointF &point) {
    UNUSED(point);
    active = true;
    activeVertex = UNSELECTED;
}

/*!
  \reimp
  */
bool FreeDrawSelect::isInside(const QPointF &point) const {
    UNUSED(point);
    return false;
}

/*!
  \reimp
  */
void FreeDrawSelect::moveItem(const QPointF &oldPos, QPointF &newPos) {
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myPainter.drawLine(oldPos, newPos);
    myPainter.end();
    setMirrorMap();
}

/*!
  \reimp
  */
void FreeDrawSelect::setOpacity(qreal val) {
    SelectItem::setOpacity(val);
}

/*!
  \reimp
  */
void FreeDrawSelect::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
    UNUSED(option);
    UNUSED(widget);
    setPixmap(myPixmap);
    QGraphicsPixmapItem::paint(painter, option, widget);
}

/*!
  Sets the objects internal map to \a map without boundary checking.
  */
void FreeDrawSelect::setPointsUnchecked(QPixmap map) {
    myPixmap = map;
    setMirrorMap();
}
/*!
  \reimp
  */
void FreeDrawSelect::read(const QJsonObject &json) {
    myID = json["objectID"].toInt();
    auto const encoded = json["pixmap"].toString().toLatin1();
    myPixmap.loadFromData(QByteArray::fromBase64(encoded), "PNG");
}

/*!
  \reimp
  */
void FreeDrawSelect::resizeItem(const int vertex, QPointF &oldP, QPointF &newP) {
    UNUSED(vertex);
    UNUSED(oldP);
    UNUSED(newP);
}

/*!
  \reimp
  */
void FreeDrawSelect::toPixmap(QPainter *painter) {
    painter->drawPixmap(QPointF(0., 0.), myPixmap);
}

/*!
  \reimp
  */
void FreeDrawSelect::updatePen(QPen pen) {
    myPen = pen;
}

/*!
  \reimp
  */
void FreeDrawSelect::write(QJsonObject &json) const {
    json["objectID"] = myID;
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    myPixmap.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    json["pixmap"] = QLatin1String(encoded);
}

/*--------------------------------- Mirror ----------------------------*/
/*!
  \reimp
  */
void FreeDrawSelect::mirrorHide() const {
    if (mirror != nullptr)
        mirror->SelectItem::hide();
}

/*!
  \reimp
  */
void FreeDrawSelect::mirrorShow() const {
    if (mirror != nullptr)
        mirror->SelectItem::show();
}

/*!
  \reimp
  */
void FreeDrawSelect::setMirror(SelectItem *item) {
    mirror = dynamic_cast<FreeDrawSelect*>(item);
}

/*!
  \reimp
  */
void FreeDrawSelect::updateMirrorScene() const {
    if (mirror != nullptr)
        mirror->scene()->update();
}

/*---------------------------- End Mirror ------------------------------*/

/*!
  Adds \a points to the internal pixel map of selected points.
  */
void FreeDrawSelect::addPoints(QSharedPointer<QPixmap> points) {
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myPainter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    myPainter.drawPixmap(QPointF(0., 0.), (*points.data()));
    myPainter.end();
    setMirrorMap();
}

/*!
  Deletes points along the brush stroke defined by \a start and \a end, with
  the given \a pen, with the changes put in \a outmap.
  */
void FreeDrawSelect::deletePoints(const QPointF &start, const QPointF &end,
                                  QPen pen, QSharedPointer<QPixmap> outmap) {
    QPixmap temp = myPixmap.copy();
    myPainter.begin(&myPixmap);
    myPainter.setCompositionMode(QPainter::CompositionMode_SourceOut);
    myPainter.setPen(pen);
    myPainter.drawLine(start, end);
    myPainter.end();
    pen.setColor(myPen.color());
    myPainter.begin(&temp);
    myPainter.setPen(pen);
    myPainter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    myPainter.drawPixmap(QPointF(0., 0.), myPixmap);
    myPainter.end();
    myPainter.begin(outmap.data());
    myPainter.setPen(pen);
    myPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    myPainter.drawPixmap(QPointF(0., 0.), temp);
    myPainter.end();

    setMirrorMap();
}

/*!
  Returns the QGraphicsScene to which this item belongs.
  */
QGraphicsScene* FreeDrawSelect::scene() const {
    return SelectItem::scene();
}

/*!
  \overload FreeDrawSelect::deletePoints()
  Deletes points based on the pixmap \a map with the given \a pen.
  */
void FreeDrawSelect::deletePoints(QPen &pen, QSharedPointer<QPixmap> map) {
    myPainter.begin(&myPixmap);
    pen.setColor(Qt::red);
    myPainter.setPen(pen);
    myPainter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    myPainter.drawPixmap(QPointF(0., 0.), (*map.data()));
    myPainter.end();
    setMirrorMap();
}


/*------------------------------- Private ---------------------------*/

/*!
  Sets the bounding rectangle and pixel map of the mirror, based on the values
  of this instance.
  */
void FreeDrawSelect::setMirrorMap() {
    if (mirror != nullptr) {
        mirror->myPixmap = myPixmap;
    }
}

/*!
  Loads QPixmap \a map into the current object.
  */
void FreeDrawSelect::loadFromPixmap(const QPixmap map) {
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myRect = QRectF(0., 0., SelectItem::myBounds.width(),
                    SelectItem::myBounds.height());
    QImage img = map.toImage();
    img = img.convertToFormat(QImage::Format_RGB32);
    QRgb *rgb;
    for (int y = 0; y < img.height(); y++) {
        rgb = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < img.width(); x++) {
            if (qRed(rgb[x]) != 0 || qBlue(rgb[x]) != 0 || qGreen(rgb[x] != 0)) {
                myPainter.drawPoint(QPoint(x, y));
            } else {
                img.setPixelColor(x, y, Qt::transparent);
            }
        }
    }
    myPainter.end();
}


inline void FreeDrawSelect::setup() {
    setMirrorMap();
    QGraphicsPixmapItem::prepareGeometryChange();
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsMovable);
}

inline void FreeDrawSelect::init(QSharedPointer<Label> label) {
    setActiveVertex(0);
    mytype = SelectItem::Freedraw;
    active = true;
    if (label != nullptr)
        label->addItem(this);
    myRect = QRectF(0., 0., SelectItem::myBounds.width(),
                    SelectItem::myBounds.height());
    myPixmap = QPixmap(SelectItem::myBounds);
    myPixmap.fill(Qt::transparent);
}
/*!
  \fn QString FreeDrawSelect::baseInstructions() const

  \reimp
  */

/*!
  \fn void FreeDrawSelect::insertVertex(const int vertex, const QPointF &point)
  \reimp

  Empty function since FreeDrawSelect items have no vertices.
  */

/*!
  \fn int FreeDrawSelect::numberOfVertices() const
  \reimp

  Returns 0 since FreeDrawSelect items have no vertices.
  */

/*!
  \fn void FreeDrawSelect::removeVertex(int vertex=UNSELECTED)
  \reimp

  Empty function since FreeDrawSelect items have no vertices.
  */

/*!
  \fn void FreeDrawSelect::resetActiveVertex()
  \reimp

  Empty function since FreeDrawSelect items have no vertices.
  */

/*!
  \fn void FreeDrawSelect::rotate(const QPointF &from, const QPointF &to)
  \reimp

  Empty function since FreeDrawSelect items cannot be rotated.
  */

/*!
  \fn FreeDrawSelect* FreeDrawSelect::getMirror() const
  \reimp
  */

/*!
  \fn void FreeDrawSelect::rotateMirror() const
  \reimp

  Empty function since FreeDrawSelect items cannot be rotated.
  */

/*!
  \fn void FreeDrawSelect::setMirrorActive() const
  \reimp
  */

/*!
  \fn void FreeDrawSelect::setMirrorAdded() const
  \reimp
  */

/*!
  \fn void FreeDrawSelect::setMirrorMoved() const
  \reimp
  */

/*!
  \fn void FreeDrawSelect::setMirrorResized() const
  \reimp
  */

/*!
  \fn void FreeDrawSelect::setMirrorVertex(int vertex) const
  \reimp
  */

/*!
  \fn bool FreeDrawSelect::isVisible() const

  Returns whether the FreeDrawSelect object is visible (\c true) on the
  QGraphicsScene or not (\c false).
  */

/*!
  \fn QPixmap FreeDrawSelect::getPixmap() const

  Returns the internal pixmap.
  */
