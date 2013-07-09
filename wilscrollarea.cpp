#include "wilscrollarea.h"

#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>
#include <QKeyEvent>


WilScrollArea::WilScrollArea(QWidget *parent) : QScrollArea(parent) {
}
//*****************************************************************

void WilScrollArea::wheelEvent(QWheelEvent *event) {

    if(event->orientation() != Qt::Vertical)
        return QScrollArea::wheelEvent(event);

    int numDegrees = event->delta() / 8;
    int numstep   = numDegrees / 5;
    QScrollBar *scrollBar = this->horizontalScrollBar();

    int curVal  = scrollBar->value();
    int nextVal = curVal - (scrollBar->singleStep() * numstep);

    if(nextVal < scrollBar->minimum())
        nextVal = scrollBar->minimum();
    else if(nextVal > scrollBar->maximum())
        nextVal = scrollBar->maximum();

    scrollBar->setValue(nextVal);
}
//*****************************************************************

void WilScrollArea::keyPressEvent(QKeyEvent *event) {

    QScrollBar *scrollBar = this->horizontalScrollBar();
    switch(event->key()) {
    case Qt::Key_Home:
        scrollBar->setValue(scrollBar->minimum());
        break;
    case Qt::Key_End:
        scrollBar->setValue(scrollBar->maximum());
        break;
    default:
        return QScrollArea::keyPressEvent(event);
    }
}
//*****************************************************************
