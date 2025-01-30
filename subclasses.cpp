#include "subclasses.h"

BondMatrix::BondMatrix(qsizetype *count)
{
    m_count = count;
    m_Matrix.resize(0);
}

BondMatrix::BondMatrix(qsizetype N, qsizetype *count)
{
    m_count = count;
    m_Matrix.resize(N);
}

void BondMatrix::resize(qsizetype N)
{
    m_Matrix.resize(N);
}

void BondMatrix::squeeze()
{
    m_Matrix.squeeze();
    for (auto i : m_Matrix) {
        i.squeeze();
    }
}

void BondMatrix::addBond(qsizetype i, qsizetype j)
{
    if (i != j) {
        if (m_Matrix[i].indexOf(j) == -1)
            m_Matrix[i].append(j);
        if (m_Matrix[j].indexOf(i) == -1)
            m_Matrix[j].append(i);
    }
}

void BondMatrix::removeBond(qsizetype i, qsizetype j)
{
    m_Matrix[i].removeAll<qsizetype>(j);
    m_Matrix[j].removeAll<qsizetype>(i);
}

bool BondMatrix::getBond(qsizetype i, qsizetype j)
{
    if (m_Matrix[i].indexOf(j) > -1) {
        return true;
    } else if (m_Matrix[j].indexOf(i) > -1) {
        return true;
    }
    return false;
}

QList<qsizetype> BondMatrix::getBonds(qsizetype i)
{
    return m_Matrix[i];
}

QList<qsizetype> BondMatrix::getUpperBonds(qsizetype i)
{
    QList<qsizetype> Result;
    for (auto item : m_Matrix[i]) {
        if(item > i) {
            Result.append(item);
        }
    }
    Result.squeeze();
    std::sort(Result.begin(), Result.end());
    return Result;
}
