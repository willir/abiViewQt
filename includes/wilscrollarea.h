#ifndef WILSCROLLAREA_H
#define WILSCROLLAREA_H

#include <QScrollArea>

class WilScrollArea : public QScrollArea {
    Q_OBJECT
public:
    explicit WilScrollArea(QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:
    virtual void wheelEvent (QWheelEvent *event);
    virtual void keyPressEvent (QKeyEvent *event);
};

#endif // WILSCROLLAREA_H
