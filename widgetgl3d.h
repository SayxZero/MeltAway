#ifndef WIDGETGL3D_H
#define WIDGETGL3D_H

#include <QMouseEvent>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QOpenGlWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>

//#include <QTime>

#include "model.h"

struct PointGL3D {
    PointGL3D() {}
    PointGL3D(QVector3D v, QVector3D n, float ch) : vertex(v), normal(n), chosen(ch) {}
    QVector3D vertex;
    QVector3D normal;
    float chosen;
};

struct SelectionRectGL3D {
    SelectionRectGL3D() { clear(); }
    SelectionRectGL3D(QVector2D p1, QVector2D p2) { setP1(p1.x(), p1.y()); setP2(p2.x(), p2.y()); }
    SelectionRectGL3D(float x1, float y1, float x2, float y2) { setP1(x1, y1); setP2(x2, y2); }

    void setCoords(QVector2D p1, QVector2D p2) { setP1(p1.x(), p1.y()); setP2(p2.x(), p2.y()); }
    void setCoords(float x1, float y1, float x2, float y2) { setP1(x1, y1); setP2(x2, y2); }

    void setP1(float x1, float y1) { startPoint = QPointF(x1, y1); }
    void setP2(float x2, float y2) {
        QPointF pnt1 = startPoint;
        QPointF pnt2 = QPointF(x2, y2);
        bool left = pnt1.x() > pnt2.x() ? true :false;
        bool bottom = pnt1.y() > pnt2.y() ? true :false;
        if (bottom) {
            if (left) {
                rect[0] = QPointF(pnt1.x(), pnt2.y()); // leftTop
                rect[1] = pnt2; // leftBottom
                rect[2] = pnt1; // rightTop
                rect[3] = QPointF(pnt2.x(), pnt1.y()); // rightBottom
            } else {
                rect[0] = pnt1; // leftTop
                rect[1] = QPointF(pnt2.x(), pnt1.y()); // leftBottom
                rect[2] = QPointF(pnt1.x(), pnt2.y()); // rightTop
                rect[3] = pnt2; // rightBottom
            }
        } else {
            if (left) {
                rect[0] = pnt2; // leftTop
                rect[1] = QPointF(pnt1.x(), pnt2.y()); // leftBottom
                rect[2] = QPointF(pnt2.x(), pnt1.y()); // rightTop
                rect[3] = pnt1; // rightBottom
            } else {
                rect[0] = QPointF(pnt2.x(), pnt1.y()); // leftTop
                rect[1] = pnt1; // leftBottom
                rect[2] = pnt2; // rightTop
                rect[3] = QPointF(pnt1.x(), pnt2.y()); // rightBottom
            }
        }
    }

    void setP1(int x1, int y1, int width, int height) {
        beginPoint = QPoint(x1, y1);
        startPoint = QPointF((x1 - qreal(width) / 2) * 2 / qreal(width),
                             ((qreal(height) - y1) - qreal(height) / 2) * 2 / qreal(height));
    }

    void setP2(int x2, int y2, int width, int height) {
        if (x2 < 0)
            x2 = 0;
        else if (x2 > width)
            x2 = width;
        if (y2 < 0)
            y2 = 0;
        else if (y2 > height)
            y2 = height;
        endPoint = QPoint(x2, y2); setP2((x2 - qreal(width) / 2) * 2 / qreal(width),
              ((qreal(height) - y2) - qreal(height) / 2) * 2 / qreal(height));
    }

    QPointF leftTop() { return rect[0]; } // 0
    QPointF leftBottom() { return rect[1]; } // 1
    QPointF rightTop() { return rect[2]; } // 2
    QPointF rightBottom() { return rect[3]; } // 3

//    int minX(int width) { return int(/*floor*/((rightTop().x() * width + width) / 2)); }
//    int maxX(int width) { return int(/*ceil*/((leftBottom().x() * width + width) / 2)); }
//    int minY(int height) { return int(/*floor*/(height - (leftTop().y() * height + height) / 2)); }
//    int maxY(int height) { return int(/*ceil*/(height - (rightBottom().y() * height + height) / 2)); }

    int minX() { return beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x(); }
    int maxX() { return beginPoint.x() > endPoint.x() ? beginPoint.x() : endPoint.x(); }
    int minY() { return beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y(); }
    int maxY() { return beginPoint.y() > endPoint.y() ? beginPoint.y() : endPoint.y(); }

    bool isEmpty() { return (rect[0] == QPointF(-2, -2) || rect[1] == QPointF(-2, -2) || rect[2] == QPointF(-2, -2) || rect[3] == QPointF(-2, -2)) ? true : false; }
    void clear() { rect[0] = QPointF(-2, -2); rect[1] = QPointF(-2, -2); rect[2] = QPointF(-2, -2); rect[3] = QPointF(-2, -2); startPoint = QPointF(-2, -2); endPoint = QPoint(-2, -2); beginPoint = QPoint(-2, -2); }

    QPointF rect[4];
    QPointF startPoint = QPointF(-2, -2);
    QPoint beginPoint = QPoint(-2, -2);
    QPoint endPoint = QPoint(-2, -2);
};

class WidgetGL3D : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    WidgetGL3D(QWidget *parent = nullptr);
    void setGeometry(QString line);
    void setViewType(int type);
    int modelSize() { return m_model.size(0); }
    QVector3D m_ModelCoords;
    void select(QPoint point);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    QVector3D unproject(QPoint point);

private:
    void setupModelVertexAttribs(QOpenGLBuffer *buffer);
    void selectByRect();
    void unselectAll();
    void updateDepthBuffer(int x_min, int y_min, int x_max, int y_max);
    void updateSelectionColorBuffer(int x_min, int y_min, int x_max, int y_max);

    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_modelMatrix;
    int m_projMatrixLoc = 0;
    int m_mvMatrixLoc = 0;
    int m_normalMatrixLoc = 0;
    int m_lightPosLoc = 0;

//    QTime m_timeMarker;
//    quint64 m_timeMarker;
    QPoint m_lastPos;
    SelectionRectGL3D m_selection;
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;
    QQuaternion m_rotation;
    QVector3D m_translation = QVector3D(0, 0, -10);
    float m_Scale = 1;

    bool flag = false;
    bool m_unprojectable = false;
    bool m_selectionFlag = false;
    bool m_miltiselectionFlag = false;
    bool m_altSelectionFlag = false;
    bool m_selectionDrawFilterFlag = false;

    float* m_depthBuffer = nullptr;
    unsigned char* m_selectionColorBuffer = nullptr;

    uint m_viewType = 0;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLShaderProgram m_selectionShaderProgram;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QVector4D bgColor = QVector4D(0.9f, 0.9f, 0.9f, 1.0f);

    QVector<qsizetype> m_queue;

    Model m_model;

//signals:

};

#endif // WIDGETGL3D_H
