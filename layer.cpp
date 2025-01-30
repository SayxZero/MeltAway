#include "layer.h"

Layer::Layer(QVector<bool>** chosen)
{
    m_Points.resize(0);
    m_ScaleFactor = 0;
    if (chosen != nullptr) {
        m_selected = new QVector<bool>();
        *chosen = m_selected;
    }
    float max = std::numeric_limits<float>::max();
    for (int i = 0; i < 3; i++) {
        m_minmax[i][0] = max;
        m_minmax[i][1] = 0.0f;
    }
}

Layer::~Layer() {
    float max = std::numeric_limits<float>::max();
    for (int i = 0; i < 3; i++) {
        m_minmax[i][0] = max;
        m_minmax[i][1] = 0.0f;
    }
}

Layer::Layer(qsizetype length, QVector<bool> **chosen)
{
    if (length > 0) {
        m_Points.resize(length * 2);
    } else {
        m_Points.resize(0);
    }
    if (chosen != nullptr) {
        m_selected = new QVector<bool>();
        *chosen = m_selected;
    }
    float max = std::numeric_limits<float>::max();
    for (int i = 0; i < 3; i++) {
        m_minmax[i][0] = max;
        m_minmax[i][1] = 0.0f;
    }
}


void Layer::add(float x, float y, float z, float nx, float ny, float nz)
{
    const QVector3D v = QVector3D(x, y, z);
    const QVector3D n = QVector3D(nx, ny, nz);

    bool found = false;
    m_total_count++;

    QPair<QMultiHash<qsizetype, qsizetype>::iterator, QMultiHash<qsizetype, qsizetype>::iterator> borders = m_PointHash.equal_range(hash(v.length()));
    QMultiHash<qsizetype, qsizetype>::iterator iter;
    for (iter = borders.first; iter != borders.second; iter++) {
        if (m_Points[iter.value() * 2] == v) {
            found = true;
            break;
        }
    }

    if (!found) {
        m_Points.append(v);
        m_Points.append(n);
        m_PointHash.insert(hash(v.length()), m_Points.length() / 2 - 1);
        m_Indices.append(m_Points.length() / 2 - 1);
        if (m_selected != nullptr) {
            m_selected->append(0);
        }
        setMinMax(v);
    } else {
        m_Points[iter.value() * 2 - 1] += n;
        m_Indices.append(iter.value());
    }
}

void Layer::add(const QVector3D &v, const QVector3D &n)
{
    bool found = false;
    m_total_count++;

    QPair<QMultiHash<qsizetype, qsizetype>::iterator, QMultiHash<qsizetype, qsizetype>::iterator> borders = m_PointHash.equal_range(hash(v.length()));
    QMultiHash<qsizetype, qsizetype>::iterator iter;
    for (iter = borders.first; iter != borders.second; iter++) {
        if (m_Points[iter.value() * 2] == v) {
            found = true;
            break;
        }
    }

    if (!found) {
        m_Points.append(v);
        m_Points.append(n);
        m_PointHash.insert(hash(v.length()), m_Points.length() / 2 - 1);
        m_Indices.append(m_Points.length() / 2 - 1);
        if (m_selected != nullptr) {
            m_selected->append(false);
        }
        setMinMax(v);
    } else {
        m_Points[iter.value() * 2 + 1] += n;
        m_Indices.append(iter.value());
    }
}

QVector3D Layer::getNear(QVector3D v)
{
    QVector3D closest = m_Points[0];
    float dist = v.distanceToPoint(closest);
    for (int i = 2; i < m_Points.length(); i += 2) {
        QVector3D pnt = m_Points[i];
        float dist0 = v.distanceToPoint(pnt);
        if (dist0 < dist) {
            dist = dist0;
            closest = pnt;
        }
    }
    return closest;
}

qsizetype Layer::getNearNum(QVector3D v)
{
    int res = 0;
    QVector3D closest = m_Points[res];
    float dist = v.distanceToPoint(closest);
    for (int i = 2; i < m_Points.length(); i += 2) {
        QVector3D pnt = m_Points[i];
        float dist0 = v.distanceToPoint(pnt);
        if (dist0 < dist) {
            res = i;
            dist = dist0;
            closest = pnt;
        }
    }
    return res / 2;
}

void Layer::squeeze()
{
    m_Points.squeeze();
    m_Indices.squeeze();
    m_PointHash.squeeze();
//    m_BondMatrix.squeeze();
}

void Layer::normalizePoints()
{
    for (uint i = 1; i < m_Points.length(); i += 2) {
        m_Points[i].normalize();
    }
}

void Layer::setMinMax(QVector3D v)
{
    if (v.x() < m_minmax[0][0])
        m_minmax[0][0] = v.x() - abs(v.x() * 0.1);
    if (v.x() > m_minmax[0][1])
        m_minmax[0][1] = v.x() + abs(v.x() * 0.1);
    if (v.y() < m_minmax[1][0])
        m_minmax[1][0] = v.y() - abs(v.y() * 0.1);
    if (v.y() > m_minmax[1][1])
        m_minmax[1][1] = v.y() + abs(v.y() * 0.1);
    if (v.z() < m_minmax[2][0])
        m_minmax[2][0] = v.z() - abs(v.z() * 0.1);
    if (v.z() > m_minmax[2][1])
        m_minmax[2][1] = v.z() + abs(v.z() * 0.1);

}

void Layer::addPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat nx, GLfloat ny, GLfloat nz)
{
//    GLfloat *ptr = m_Buffer.data() + m_buffer_count;
//    *ptr++ = x;
//    *ptr++ = y;
//    *ptr++ = z;
//    *ptr++ = nx;
//    *ptr++ = ny;
//    *ptr++ = nz;
//    m_buffer_count += 6;
}

