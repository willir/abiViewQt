#include "graphwidget.h"

#include <QMessageBox>
#include <typeinfo>
#include <QPalette>
#include <QStyleOption>
#include <QDebug>
#include <QMouseEvent>
#include <qmath.h>

#include <math.h>
#include <fstream>

#include "ajseqabi.h"
#include "ajmess.h"
#include "ajfile.h"

using namespace std;

const std::map<char, Qt::GlobalColor> GraphWidget::BASES_MAP_COLOR {
        {'A', Qt::green},
        {'T', Qt::red},
        {'G', Qt::black},
        {'C', Qt::blue}
};

GraphWidget::GraphWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    this->trace          = NULL;
    this->nseq           = NULL;
    this->basePositions  = NULL;
    this->hideTrash      = false;
}
//*****************************************************************

GraphWidget::~GraphWidget() {
    this->clearEmbosVars();
}
//*****************************************************************

void GraphWidget::clearEmbosVars() {
    ajInt2dDel(&this->trace);
    ajStrDel(&this->nseq);
    ajShortDel(&this->basePositions);
}
//*****************************************************************

void GraphWidget::printAjInt2d(AjPInt2d val, const char *tag) {
    ajuint len1, len2;
    ajInt2dLen(val, &len1, &len2);

    qDebug("%s Len:%d:%d", tag, len1, len2);

#if 0
    QDebug dbg(QtDebugMsg);
    for(ajuint i=0; i<len2; i++) {
        for(ajuint j=0; j<len1; j++) {
            dbg << ajInt2dGet(val, j, i);
        }
        dbg << endl;
    }
#endif
}
//*****************************************************************

void GraphWidget::printAjShort(AjPShort arr, const char* tag) {
    ajuint len;
    len = ajShortLen(arr);

    qDebug() << tag << " Len:" << len;

    QDebug dbg(QtDebugMsg);
    for (ajuint i=0; i<len; i++) {
        dbg << ajShortGet(arr, i);
    }
}
//*****************************************************************

ajint GraphWidget::findTraceMax() {
    ajint res = 0;

    for(ajint iloop=0; iloop<numPoints; iloop++) {
        for(ajint ibase=0; ibase<4; ibase++) {
            if(res < ajInt2dGet(trace, ibase, iloop))
                res = ajInt2dGet(trace, ibase, iloop);
        }
    }
    return res;
}
//*****************************************************************

ajlong GraphWidget::findLastNonTrashPoint() {
    ajlong seqLen = ajShortLen(this->basePositions);
    return ajShortGet(this->basePositions, seqLen-1) + 10;
}
//*****************************************************************

bool GraphWidget::isSequenceLoaded() {
    return this->trace != NULL;
}
//*****************************************************************

void GraphWidget::setNumVisiblePoints() {
    if(this->hideTrash && this->lastNonTrashPoint != 0)
        this->numVisiblePoints = this->lastNonTrashPoint;
    else
        this->numVisiblePoints = this->numPoints;
}
//*****************************************************************

bool GraphWidget::readSeqFrom(const char *fileName) {
    ajlong    numBases;
    ajlong    baseO;
    ajlong    basePosO;
    ajlong    fwo_;
    ajlong    dataOffset[4]  = {0};
    AjPFile   fp             = NULL;

    /* BYTE[i] is a byte mask for byte i */
    const ajlong BYTE[] = { 0x000000ff };

    qDebug() << "readSeqFrom" << endl;

    this->clearEmbosVars();

    fp = ajFileNewInNameC(fileName);

    numBases = ajSeqABIGetNBase(fp);
    baseO = ajSeqABIGetBaseOffset(fp);	/* find BASE tag & get offset */
    nseq  = ajStrNew();			/* read in sequence */

    trace = ajInt2dNew();
    basePositions = ajShortNew();
    numPoints = ajSeqABIGetNData(fp);  /* find DATA tag & get no. of points */

    ajSeqABIGetTraceOffset(fp, dataOffset);
    ajSeqABIGetData(fp, dataOffset, numPoints, trace); /* read in trace data   */

    fwo_ = ajSeqABIGetFWO(fp);	       /* find FWO tag - field order GATC   */

    basesOrder[0] = (char) (fwo_ >> 24 & BYTE[0]);
    basesOrder[1] = (char) (fwo_ >> 16 & BYTE[0]);
    basesOrder[2] = (char) (fwo_ >> 8  & BYTE[0]);
    basesOrder[3] = (char) (fwo_ & BYTE[0]);

    ajSeqABIReadSeq(fp, baseO, numBases, &nseq);
    basePosO = ajSeqABIGetBasePosOffset(fp); /* find PLOC tag & get offset */
    ajFileSeek(fp, basePosO, SEEK_SET);
    ajSeqABIGetBasePosition(fp, numBases, &basePositions);

    this->tmax = this->findTraceMax();
    this->lastNonTrashPoint = this->findLastNonTrashPoint();

    this->setNumVisiblePoints();

    qDebug("The Genome Sequence Len:%lu", ajStrGetLen(nseq));
    ajUser("The Genome Sequence: %S\n", nseq);

    qDebug() << ajFileGetNameC(fp);
    qDebug().nospace() << "base0:" << baseO << " basePosO:" << basePosO;
    qDebug().nospace() << "numPoints:" << numPoints << " tmax:" << tmax;
    qDebug().nospace() << "lastNonTrashPoint:" << lastNonTrashPoint;
    qDebug().nospace() << basesOrder[0] << basesOrder[1] << basesOrder[2] << basesOrder[3];

    printAjShort(basePositions, "basePositions");
    printAjInt2d(trace, "trace");

    goto end;
end:
    ajFileClose(&fp);

    return true;
}
//*****************************************************************

void GraphWidget::drawSequenceGraph(QPainterPath *path, ajint ibase) {
    const int yMax = height() - GRAPH_HEADER_HEIGHT;

    path->moveTo(0, 0);

    fstream yOrds ("/home/willir/tmp2/test.txt", fstream::out);

    for(int x = 0; x < this->numPoints; ++x) {
        float y = (float) ajInt2dGet(this->trace, ibase, x);
        int yNormalized = (int) (y / this->tmax * yMax);
        yNormalized = yMax - yNormalized + GRAPH_HEADER_HEIGHT;

        QPointF point(GRPAH_PADDING_LEFT + x, yNormalized);

        if(x == 0)
            path->moveTo(point);
        else
            path->lineTo(point);

        yOrds << x << ":" << y << " ";
    }
    yOrds.close();
}
//*****************************************************************

void GraphWidget::drawSequenceText(QPainter *p) {
    ajlong seqLen = ajStrGetLen(this->nseq);

    QFont font("Courier New");
    font.setPixelSize(14);
    p->setFont(font);

    int charHalfWidth = QFontMetrics(font).width("C") / 2;

    for(int ibase=0; ibase<seqLen; ibase++) {
        int xPos = GRPAH_PADDING_LEFT + ajShortGet(this->basePositions, ibase) - charHalfWidth;
        char seqChar = ajStrGetCharPos(this->nseq, ibase);
        QString txt(seqChar);

        p->setPen(BASES_MAP_COLOR.at(seqChar));
        p->drawText(xPos, GRAPH_TEXT_Y_POS, txt);
    }
}
//*****************************************************************

void GraphWidget::drawMouseVerticalLine(QPainter *p) {
    if(this->presedMousePos.isNull())
        return;

    p->setPen(QPen(GRAPH_VERICAL_MOUSE_LINE_COLOR, 1, Qt::SolidLine));

    QPoint top(this->presedMousePos.x(), 0);
    QPoint bottom(this->presedMousePos.x(), height());
    p->drawLine(top, bottom);
}
//*****************************************************************

void GraphWidget::paintEvent(QPaintEvent *) {

    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    if(!this->isSequenceLoaded())
        return;

    setMinimumWidth(GRPAH_PADDING_LEFT + this->numVisiblePoints);

    for(ajint ibase=0; ibase<4; ++ibase) {
        Qt::GlobalColor color = BASES_MAP_COLOR.at(basesOrder[ibase]);
        p.setPen(QPen(color, 1, Qt::SolidLine)); // Draw settings

        QPainterPath path;
        this->drawSequenceGraph(&path, ibase);
        p.drawPath(path);
    }
    this->drawSequenceText(&p);
    this->drawMouseVerticalLine(&p);
}
//*****************************************************************

void GraphWidget::mousePressEvent(QMouseEvent *event) {
    if(!this->isSequenceLoaded())
        return;

    this->presedMousePos = event->pos();
    this->update();
}
//*****************************************************************

void GraphWidget::mouseReleaseEvent(QMouseEvent *) {
    if(!this->isSequenceLoaded())
        return;

    this->presedMousePos = QPoint();
    this->update();
}
//*****************************************************************

void GraphWidget::mouseMoveEvent(QMouseEvent *event) {
    if(!this->isSequenceLoaded() || this->presedMousePos.isNull())
        return;

    this->presedMousePos = event->pos();
    this->update();
}
//*****************************************************************

void GraphWidget::hideTrashStateChanged(int state) {
    this->hideTrash = state != 0;
    if(!this->isSequenceLoaded())
        return;

    this->setNumVisiblePoints();
    this->update();
}
//*****************************************************************
