#ifndef LAYER_H
#define LAYER_H

#include <qopengl.h>
#include <QList>
#include <QVector3D>
#include <QList>
#include <QDebug>
#include <QMultiHash>
#include <QSet>
#include "subclasses.h"


class Layer
{

public:
    void extracted(float &max);
    Layer(QVector<bool> **chosen = nullptr);
    Layer(qsizetype length, QVector<bool> **chosen = nullptr);
    ~Layer();
    void add(float x, float y, float z, float nx, float ny, float nz);
    void add(const QVector3D &v, const QVector3D &n);
    QVector3D get(qsizetype i) { return m_Points[i]; }
    QVector3D getNear(QVector3D v);
    qsizetype getNearNum(QVector3D v);
    void *getMinMax() {return &m_minmax; }
    qsizetype size() const { return m_Points.count(); }
    //    int vertexcount() const { return m_NodesPnts.size(); }
    QVector<QVector3D> *vertData() { return &m_Points; }
    QVector<GLuint> *indData() { return &m_Indices; }
    int indexcont() const { return m_Indices.size(); }
    float ScaleFactor() { return m_ScaleFactor; }
    void squeeze();
    void normalizePoints();
    QVector<bool> *m_selected = nullptr;

private:
//    qsizetype m_count = 0;
    void setMinMax(QVector3D v);
    QMultiHash<qsizetype, qsizetype> m_PointHash;

    QVector<QVector3D> m_Points;
    QList<uint> m_Indices;

    qsizetype m_total_count = 0;
//    BondMatrix m_BondMatrix = BondMatrix(&m_count);
    float m_minmax[3][2];

    void addPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat nx, GLfloat ny, GLfloat nz);

    float m_ScaleFactor;

    long long hash(float f) { return int64_t(f * 1000000); };
};

#endif // LAYER_H
