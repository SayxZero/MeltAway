#include "widgetgl3d.h"
#include <QApplication>
#include <math.h>
#include <QDebug>

WidgetGL3D::WidgetGL3D(QWidget *parent)
    : QOpenGLWidget(parent),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

}

void WidgetGL3D::setGeometry(QString line)
{
    m_model.initModel(line);
    m_shaderProgram.bind();

    const QVector<QVector3D> *vertices = m_model.vertData(0);
    const QVector<GLuint> *indices = m_model.indData(0);
    QVector<PointGL3D> *points = new QVector<PointGL3D>();

    for (int ind = 0; ind < vertices->size(); ind += 2) {
        points->append(PointGL3D(vertices->at(ind), vertices->at(ind + 1), 0.0f));
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(points->constData(), points->size() * sizeof(PointGL3D));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indices->constData(), indices->size() * sizeof(GLuint));
    m_indexBuffer.release();

    m_shaderProgram.setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    setupModelVertexAttribs(&m_vertexBuffer);
    m_shaderProgram.release();

    m_Scale = m_model.ScaleFactor() * 10;

    delete points;

    update();

}

void WidgetGL3D::setViewType(int type)
{
    m_viewType = type;
    update();
}

void WidgetGL3D::select(QPoint point)
{
    QVector3D vert = unproject(point);
    qDebug() << "Point clicked";
    qDebug() << point.x();
    qDebug() << point.y();
    qDebug() << "";
    if (m_depthBuffer[point.x() + point.y() * width()] != 1.0) {
//        qsizetype ind = m_model.invertSelected(m_ModelCoords);
//        m_queue.append(ind);

//        qsizetype ind = m_model.getNearNum(m_ModelCoords);

        qsizetype ind = m_model.getNearNum(vert);

        m_model.invertSelected(ind);
        m_queue.append(ind);
    }
}

void WidgetGL3D::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(bgColor.x(), bgColor.y(), bgColor.z(), bgColor.w());
//    glFrustum(-.1, .1, -.1, .1, .2, 1000);
//    glFrustum(-1, 1, -1, 1, 2, 100);

    //    glEnable(GL_CULL_FACE);
    //qDebug() << m_frameBuffer->isValid();
//    m_frameBuffer = new QOpenGLFramebufferObject(rect().size());
//    m_frameBuffer->bind();
    //glEnable(GL_DOUBLEBUFFER);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    // Сглаживание линий
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    // Сглаживание полигонов
    //    glEnable(GL_POLYGON_SMOOTH);
    //    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.vsh");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.fsh");
    m_shaderProgram.link();
    //m_shaderProgram.bind();

    m_selectionShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/svshader.vsh");
    m_selectionShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/sfshader.fsh");
    m_selectionShaderProgram.link();

    const QVector<QVector3D> *vertices = m_model.vertData(0);
    const QVector<GLuint> *indices = m_model.indData(0);
    QVector<PointGL3D> *points = new QVector<PointGL3D>();

    for (int ind = 0; ind < vertices->size(); ind += 2) {
        points->append(PointGL3D(vertices->at(ind), vertices->at(ind + 1), 0.0f));
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(points->constData(), points->size() * sizeof(PointGL3D));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indices->constData(), indices->size() * sizeof(GLuint));
    m_indexBuffer.release();

    setupModelVertexAttribs(&m_vertexBuffer);

    m_shaderProgram.setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));
    m_shaderProgram.release();

    delete points;
}

void WidgetGL3D::paintGL()
{

    if (m_selectionDrawFilterFlag) {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_selectionShaderProgram.bind();

        m_modelMatrix.setToIdentity();
        //        m_modelMatrix.scale(m_Scale);
        m_viewMatrix.setToIdentity();
        m_viewMatrix.translate(m_translation);
        m_viewMatrix.rotate(m_rotation);
        m_viewMatrix.scale(m_Scale);

        m_selectionShaderProgram.setUniformValue("u_projectionMatrix", m_projectionMatrix);
        m_selectionShaderProgram.setUniformValue("u_viewMatrix", m_viewMatrix);
        m_selectionShaderProgram.setUniformValue("u_modelMatrix", m_modelMatrix);
        m_selectionShaderProgram.setUniformValue("u_bgColor", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

        glPolygonMode(GL_FRONT, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POINT_SMOOTH);
        glDisable(GL_BLEND);

        m_indexBuffer.bind();
        m_vertexBuffer.bind();

        m_selectionShaderProgram.setUniformValue("u_elementType", 0);
        glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

        glPolygonMode(GL_FRONT, GL_POINT);
        glPointSize(1);
        m_selectionShaderProgram.setUniformValue("u_elementType", 1);
        glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

        m_indexBuffer.release();
        m_vertexBuffer.release();

        //glReadPixels(m_selection.minX(), m_selection.minY(), m_selection.maxX(),  m_selection.maxY(), GL_DEPTH_COMPONENT, GL_FLOAT, m_depthBuffer);


        m_selectionShaderProgram.release();

//        qDebug() << m_selection.maxX();

//        updateDepthBuffer(m_selection.minX(), m_selection.minY(), m_selection.maxX(),  m_selection.maxY());
        updateSelectionColorBuffer(m_selection.minX(), m_selection.minY(), m_selection.maxX(),  m_selection.maxY());


    }

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    // Сглаживание линий
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

    glClearColor(bgColor.x(), bgColor.y(), bgColor.z(), bgColor.w());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DOUBLEBUFFER);

        switch (m_viewType) {

        case 1:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_CULL_FACE);
            glDisable(GL_LINE_SMOOTH);
            break;

        default:
            glPolygonMode(GL_FRONT, GL_FILL);
            glEnable(GL_CULL_FACE);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
            break;

        }


    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_shaderProgram.setUniformValue("u_viewMatrix", m_viewMatrix);
    m_shaderProgram.setUniformValue("u_modelMatrix", m_modelMatrix);
    m_shaderProgram.setUniformValue("u_bgColor", bgColor);

        m_modelMatrix.setToIdentity();
    //        m_modelMatrix.scale(m_Scale);
        m_viewMatrix.setToIdentity();
        m_viewMatrix.translate(m_translation);
        m_viewMatrix.rotate(m_rotation);
        m_viewMatrix.scale(m_Scale);


        m_vertexBuffer.bind();
        m_indexBuffer.bind();

        if (!m_queue.isEmpty()) {
            for (auto num : m_queue) {
                float data = m_model.getSelected(num) ? 1.0f : 0.0f;
                m_vertexBuffer.write(sizeof(PointGL3D) * num + 2 * sizeof(QVector3D), &data, sizeof(float));
            }
            m_queue.clear();
        }

        m_shaderProgram.setUniformValue("u_elementType", 0);
        glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

        if (m_viewType == 0) {
            glPolygonMode(GL_FRONT, GL_LINE);
            //        glLineWidth(2);
            m_shaderProgram.setUniformValue("u_elementType", 1);
            glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
        }

        glPolygonMode(GL_FRONT, GL_POINT);
        glPointSize(3);
        m_shaderProgram.setUniformValue("u_elementType", 2);
        glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
        m_indexBuffer.release();
        m_vertexBuffer.release();

        if (!m_selection.isEmpty()) {
            glPolygonMode(GL_FRONT, GL_FILL);
            glDisable(GL_CULL_FACE);
            m_shaderProgram.setUniformValue("u_elementType", 3);
            glBegin(GL_QUADS);
                glVertex2f(m_selection.leftBottom().x(), m_selection.leftBottom().y());
                glVertex2f(m_selection.leftTop().x(), m_selection.leftTop().y());
                glVertex2f(m_selection.rightTop().x(), m_selection.rightTop().y());
                glVertex2f(m_selection.rightBottom().x(), m_selection.rightBottom().y());
            glEnd();

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            m_shaderProgram.setUniformValue("u_elementType", 4);
            glBegin(GL_QUADS);
                glVertex2f(m_selection.leftBottom().x(), m_selection.leftBottom().y());
                glVertex2f(m_selection.leftTop().x(), m_selection.leftTop().y());
                glVertex2f(m_selection.rightTop().x(), m_selection.rightTop().y());
                glVertex2f(m_selection.rightBottom().x(), m_selection.rightBottom().y());
            glEnd();

            glEnable(GL_CULL_FACE);
        }

        m_shaderProgram.release();

        updateDepthBuffer(0, 0, width(), height());

        m_ModelCoords = unproject(m_lastPos);

}

void WidgetGL3D::resizeGL(int width, int height)
{
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45.0f, GLfloat(width) / height, 0.01f, 100.0f);
}

void WidgetGL3D::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->position().toPoint();
    if (event->buttons() & Qt::LeftButton) {
        m_selectionFlag = true;
        m_selection.setP1(event->position().x(), event->position().y(), width(), height());
    }
    else if (event->buttons() & Qt::RightButton) {

    }
    update();
}

void WidgetGL3D::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        m_selection.setP2(event->position().x(), event->position().y(), width(), height());
    }
    else if (event->buttons() & Qt::RightButton) {

    }

    else if (m_selectionFlag) {
        if (!m_miltiselectionFlag && !m_altSelectionFlag)
            unselectAll();
        if (m_selection.isEmpty()) {
            select(event->position().toPoint());
        } else {
            m_selectionDrawFilterFlag = true;
            repaint();
            selectByRect();
        }
        m_selectionFlag = false;
    }
    m_selection.clear();
    update();
}

void WidgetGL3D::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D diff = QVector2D(event->position().toPoint() - m_lastPos);

    if (event->buttons() & Qt::LeftButton) {
//        unselectAll();
        if (m_selection.startPoint == QPointF(-2.0, -2.0)) {
            m_selection.setP1(event->position().x(), event->position().y(), width(), height());
//            m_selection.setP1((event->position().x() - qreal(rect().width()) / 2) * 2 / qreal(rect().width()),
//                              ((qreal(rect().height()) - event->position().y()) - qreal(rect().height()) / 2) * 2 / qreal(rect().height()));
        } else {
            m_selection.setP2(event->position().x(), event->position().y(), width(), height());
//            m_selection.setP2((event->position().x() - qreal(rect().width()) / 2) * 2 / qreal(rect().width()),
//                              ((qreal(rect().height()) - event->position().y()) - qreal(rect().height()) / 2) * 2 / qreal(rect().height()));
        }
        update();
    } else if (event->buttons() & Qt::RightButton) {
        float angle = diff.length() / 2.0f;
        QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);
        m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;
        update();
    } else if (event->buttons() & Qt::MiddleButton) {
        m_translation += QVector3D(diff.x() / 100, - diff.y() / 100, 0.0f);
        update();
    } else {
        m_lastPos = event->position().toPoint();
        update();
    }
    m_lastPos = event->position().toPoint();
}

void WidgetGL3D::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta();

    if (!numPixels.isNull()) {
            m_Scale = m_Scale + float(numPixels.y()) / 100;
    } else if (!numDegrees.isNull()) {
            QPoint numSteps = numDegrees;
            float(numSteps.y()) > 0 ? m_Scale = m_Scale * (float(numSteps.y()) / 100.0f) : m_Scale = m_Scale / (- float(numSteps.y()) / 100.0f);
//            m_translation += QVector3D(0, 0, float(numSteps.y()) / 100.0f);
    }
    update();
}

void WidgetGL3D::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        m_miltiselectionFlag = true;
    }
    if (event->key() == Qt::Key_Alt) {
        m_altSelectionFlag = true;
    }
}

void WidgetGL3D::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        m_miltiselectionFlag = false;
    }
    if (event->key() == Qt::Key_Alt) {
        m_altSelectionFlag = false;
    }
}

QVector3D WidgetGL3D::unproject(QPoint point)
{
//    float z;
    int x = point.x();
    int y = point.y();
    QRect VP = rect();
    y = VP.height() - y - 1;
//    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
//    if (z == 1.0f)
//        m_unprojectable = true;
//    else
//        m_unprojectable = false;
//    QVector3D result(float(x), float(y), z);

    QVector3D result(float(x), float(y), 1);
    if (x < width() && x > 0 && y < height() && y > 0)
        result = QVector3D(float(x), float(y), m_depthBuffer[x + y * width()]);
    return result.unproject(m_viewMatrix * m_modelMatrix, m_projectionMatrix, VP);
}

void WidgetGL3D::setupModelVertexAttribs(QOpenGLBuffer *buffer)
{
//    m_modelVAO.bind();
    buffer->bind();
    m_shaderProgram.setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointGL3D),
                             nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PointGL3D),
                             reinterpret_cast<void *>(sizeof(QVector3D)));
    f->glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(PointGL3D),
                             reinterpret_cast<void *>(2 * sizeof(QVector3D)));
    buffer->release();
    //    m_modelVAO.release();
}

void WidgetGL3D::selectByRect()
{
//    qDebug() << m_selection.minX();
//    qDebug() << m_selection.maxX();
//    qDebug() << m_selection.minY();
//    qDebug() << m_selection.maxY();

    if (m_viewType != 0) {
        for (qsizetype i = 0; i < m_model.size(0) / 2; i++) {
            QVector3D vert = m_model.getPoint(0, i);
            QVector3D projection = m_projectionMatrix.map(m_viewMatrix.map(m_modelMatrix.map(vert)));
            if (projection.x() > m_selection.leftTop().x() && projection.x() < m_selection.rightBottom().x()
                && projection.y() > m_selection.rightBottom().y() && projection.y() < m_selection.leftTop().y())
            {
                if (!m_altSelectionFlag)
                    m_model.setSelected(i);
                else
                    m_model.setUnselected(i);
                m_queue.append(i);
            }
        }
    } else {
        for (qsizetype i = m_selection.minX(); i < m_selection.maxX(); i++) {
            for (qsizetype j = m_selection.minY(); j < m_selection.maxY(); j++) {
//                qDebug() << m_selectionColorBuffer[(i - m_selection.minX() + (j - m_selection.minY()) * (m_selection.maxY() - m_selection.minY()))];
                if (m_selectionColorBuffer[(i - m_selection.minX() + (j - m_selection.minY()) * (m_selection.maxY() - m_selection.minY()))] == char(255)) {
//                int ii = (i - m_selection.minX()) + (j - m_selection.minY()) * (m_selection.maxY() - m_selection.minY()) * 3;
//                qDebug() << ii;
                    QVector3D vert = unproject(QPoint(i, j));
                    qDebug() << "Pnt";
                    qDebug() << i;
                    qDebug() << j;
                    qDebug() << "";
                    if (m_depthBuffer[QPoint(i, j).x() + QPoint(i, j).y() * width()] != 1.0) {
                        qsizetype ind = m_model.getNearNum(vert);
                        if (!m_altSelectionFlag)
                            m_model.setSelected(ind);
                        else
                            m_model.setUnselected(ind);
                        m_queue.append(ind);
                    }
                }
            }
        }
    }
    m_selectionDrawFilterFlag = false;
}


void WidgetGL3D::unselectAll()
{
    for (qsizetype i = 0; i < m_model.size(0) / 2; i++) {
        m_model.setUnselected(i);
        m_queue.append(i);
    }
    update();
}

void WidgetGL3D::updateDepthBuffer(int x_min, int y_min, int x_max, int y_max)
{
    if (m_depthBuffer != nullptr) {
        delete m_depthBuffer;
    }
    m_depthBuffer = new float [(x_max - x_min) * (y_max - y_min)];

//    y_min = height() - y_min;
//    y_max = height() - y_max;

    glReadPixels(x_min, y_min, x_max - x_min, y_max - y_min, GL_DEPTH_COMPONENT, GL_FLOAT, m_depthBuffer);
}

void WidgetGL3D::updateSelectionColorBuffer(int x_min, int y_min, int x_max, int y_max)
{
    if (m_selectionColorBuffer != nullptr) {
        delete m_selectionColorBuffer;
    }
    m_selectionColorBuffer = new unsigned char [(x_max - x_min + 2) * (y_max - y_min + 2) * 1];

    y_min = height() - y_min - 1;
    y_max = height() - y_max - 1;

    qDebug() << "selection";
    qDebug() << x_min;
    qDebug() << x_max;
    qDebug() << y_min;
    qDebug() << y_max;
    qDebug() << "";

    glReadPixels(x_min, y_min, x_max - x_min, y_max - y_min, GL_RED, GL_UNSIGNED_BYTE, m_selectionColorBuffer);
}

//void WidgetGL3D::setupSelectionVertexAttribs()
//{

//    m_selectionBuffer.release();
//    m_selectionVAO.release();
//}
