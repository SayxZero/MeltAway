// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "model.h"
#include <qmath.h>
#include <QDebug>

Model::Model()
{
//    initModel("D:/Prog/TrialOGL3D/mdls/fuell.stl");
    m_layers.append(new Layer());
}

Model::Model(QString line)
{
    initModel(line);
}

int Model::solve(float speed)
{

}

void Model::initModel(QString line)
{
    m_layers.clear();
    QFile file(line);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    int NUM = 0;
    //    while (!in.atEnd())
    //    {
    //        QString str;
    //        in >> str;
    //        if (str == "vertex") {
    //            NUM++;
    //        } else if (str == "endsolid") {
    //            break;
    //        }
    //    }
    //    in.seek(0);
    m_layers.append(new Layer(NUM, &m_selected));
    QVector3D Vertex;
    QVector3D Normal;
    float Max = 0;
    while (!in.atEnd())
    {
        QString str;
        in >> str;
        if (str == "normal") {
            in >> str;
            const GLfloat x = str.toFloat();
            in >> str;
            const GLfloat y = str.toFloat();
            in >> str;
            const GLfloat z = str.toFloat();
            Normal = QVector3D(x, y, z);
        } else if (str == "vertex") {
            in >> str;
            const GLfloat x = str.toFloat();
            in >> str;
            const GLfloat y = str.toFloat();
            in >> str;
            const GLfloat z = str.toFloat();
            Vertex = QVector3D(x, y, z);
            m_layers[0]->add(Vertex, Normal);
            float XX = Vertex.length() * 3.01;
            if (XX > Max) {
                Max = XX;
            }
        } else if (str == "endsolid") {
            break;
        }
    }
    file.close();
    m_layers[0]->squeeze();
    m_layers[0]->normalizePoints();
    m_ScaleFactor = 1 / Max;
}

qsizetype Model::invertSelected(float x, float y, float z)
{
    qsizetype num = -1;
    if (m_selected != nullptr) {
        num = m_layers[0]->getNearNum(QVector3D(x, y, z));
        bool &pnt = m_layers[0]->m_selected->data()[num];
        pnt = !pnt;
        return 0;
    }
    return num;
}

qsizetype Model::invertSelected(QVector3D v)
{
    qsizetype num = -1;
    if (m_selected != nullptr) {
        num = m_layers[0]->getNearNum(v);
        bool &pnt = m_layers[0]->m_selected->data()[num];
        pnt = !pnt;
    }
    return num;
}

qsizetype Model::getNearNum(QVector3D v)
{
    qsizetype num = -1;
    if (m_selected != nullptr) {
        num = m_layers[0]->getNearNum(v);
    }
    return num;
}
