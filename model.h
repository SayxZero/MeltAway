// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef LOGO_H
#define LOGO_H

#include <qopengl.h>
#include <QList>
#include <QVector3D>
#include <QFile>
#include <QTextStream>

#include "layer.h"

class Model
{
public:
    Model();
    Model(QString line);
    const QVector<QVector3D> *vertData(int i) { return m_layers[i]->vertData(); }
    const QVector<GLuint> *indData(int i) { return m_layers[i]->indData(); }
    int layerCount() { return m_layers.size(); }
    int solve(float speed);
    int size(qsizetype i) { return m_layers[i]->size(); }
    float ScaleFactor() { return m_ScaleFactor; }
    void initModel(QString line);
    qsizetype invertSelected(float x, float y, float z);
    qsizetype invertSelected(QVector3D v);
    qsizetype getNearNum(QVector3D v);
    void setSelected(qsizetype i) { bool &pnt = m_layers[0]->m_selected->data()[i]; pnt = true; }
    void invertSelected(qsizetype i) { bool &pnt = m_layers[0]->m_selected->data()[i]; pnt = !pnt; }
    void setUnselected(qsizetype i) { bool &pnt = m_layers[0]->m_selected->data()[i]; pnt = false; }
    int getSelected(qsizetype i) { return m_layers[0]->m_selected->at(i); }
    QVector3D getPoint(qsizetype layer, qsizetype index) { return m_layers[layer]->get(index * 2); };
    QVector3D getNormal(qsizetype layer, qsizetype index) { return m_layers[layer]->get(index * 2 + 1); };

private:
    QVector<bool>* m_selected = nullptr;
    QVector<Layer*> m_layers;
    float m_ScaleFactor;
};

#endif // LOGO_H
