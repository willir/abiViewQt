#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QPainterPath>
#include <QPainter>

#include <map>

#include "ajarch.h"
#include "ajarr.h"
#include "ajstr.h"

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    explicit GraphWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~GraphWidget();

    bool readSeqFrom(const char *fileName);
signals:

public slots:
    void hideTrashStateChanged(int state);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:

    static const int  GRPAH_PADDING_LEFT    = 8;
    static const int  GRAPH_HEADER_HEIGHT   = 40;
    static const int  GRAPH_TEXT_Y_POS      = 20;
    static const Qt::GlobalColor GRAPH_VERICAL_MOUSE_LINE_COLOR = Qt::lightGray;

    static const std::map<char, Qt::GlobalColor> BASES_MAP_COLOR;

    AjPInt2d  trace;
    AjPStr    nseq;
    AjPShort  basePositions;
    ajlong    numPoints;
    ajlong    numVisiblePoints;
    ajlong    lastNonTrashPoint;
    ajint     tmax;                // Max signal strength
    QPoint    presedMousePos;      // Not isNull if mouse was presed.
    char      basesOrder[4];       // Sequnce bases order

    bool      hideTrash;

    static void printAjInt2d(AjPInt2d val, const char *tag);
    static void printAjShort(AjPShort arr, const char* tag);

    ajint  findTraceMax();
    ajlong findLastNonTrashPoint();

    /*!
     * @return true if genome file sequence was loaded.
     */
    bool isSequenceLoaded();

    /*!
     * Calculates and sets {@link numVisiblePoints}.
     * P.S. Depends on {@link numPoints} and {@link lastNonTrashPoint}.
     */
    void setNumVisiblePoints();

    void drawSequenceGraph(QPainterPath *path, ajint ibase);
    void drawSequenceText(QPainter *p);

    /**
     * Draws vertical line at {@link presedMousePos} if it isn't null.
     */
    void drawMouseVerticalLine(QPainter *p);

    /*!
     * Frees memory of class variable for emboss library.
     * P.S. All variables will be point to NULL.
     */
    void clearEmbosVars();
};

#endif // GRAPHWIDGET_H
