#ifndef NODE_H
#define NODE_H

#include <QList>
#include <algorithm>


class BondMatrix {

public:
    BondMatrix(qsizetype *count);
    BondMatrix(qsizetype N, qsizetype *count);
    void resize(qsizetype N);
    void squeeze();
    void addBond(qsizetype i, qsizetype j);
    void removeBond(qsizetype i, qsizetype j);
    bool getBond(qsizetype i, qsizetype j);
    QList<qsizetype> getBonds(qsizetype i);
    QList<qsizetype> getUpperBonds(qsizetype i);
//    QString getStrData();

private:
    qsizetype *m_count;
    QList<QList<qsizetype>> m_Matrix;

};

#endif // NODE_H
