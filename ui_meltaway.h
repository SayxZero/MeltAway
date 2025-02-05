/********************************************************************************
** Form generated from reading UI file 'meltaway.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MELTAWAY_H
#define UI_MELTAWAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgetgl3d.h"

QT_BEGIN_NAMESPACE

class Ui_MeltAway
{
public:
    QAction *action_exit;
    QAction *action_setViewType_solid;
    QAction *action_setViewType_wire;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *OpenButton;
    WidgetGL3D *openGLWidget;
    QHBoxLayout *horizontalLayout_2;
    QSlider *horizontalSlider;
    QCheckBox *checkBox_BrushSelection;
    QMenuBar *menubar;
    QMenu *menu_file;
    QMenu *menu_view;
    QMenu *menu_viewType;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MeltAway)
    {
        if (MeltAway->objectName().isEmpty())
            MeltAway->setObjectName("MeltAway");
        MeltAway->resize(800, 600);
        action_exit = new QAction(MeltAway);
        action_exit->setObjectName("action_exit");
        action_setViewType_solid = new QAction(MeltAway);
        action_setViewType_solid->setObjectName("action_setViewType_solid");
        action_setViewType_solid->setCheckable(true);
        action_setViewType_solid->setChecked(true);
        action_setViewType_wire = new QAction(MeltAway);
        action_setViewType_wire->setObjectName("action_setViewType_wire");
        action_setViewType_wire->setCheckable(true);
        centralwidget = new QWidget(MeltAway);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout->addWidget(lineEdit);

        OpenButton = new QPushButton(centralwidget);
        OpenButton->setObjectName("OpenButton");

        horizontalLayout->addWidget(OpenButton);


        verticalLayout->addLayout(horizontalLayout);

        openGLWidget = new WidgetGL3D(centralwidget);
        openGLWidget->setObjectName("openGLWidget");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(openGLWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, 10, -1, 10);
        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName("horizontalSlider");
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(horizontalSlider);

        checkBox_BrushSelection = new QCheckBox(centralwidget);
        checkBox_BrushSelection->setObjectName("checkBox_BrushSelection");
        checkBox_BrushSelection->setEnabled(false);
        checkBox_BrushSelection->setChecked(false);

        horizontalLayout_2->addWidget(checkBox_BrushSelection);


        verticalLayout->addLayout(horizontalLayout_2);

        MeltAway->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MeltAway);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menu_file = new QMenu(menubar);
        menu_file->setObjectName("menu_file");
        menu_view = new QMenu(menubar);
        menu_view->setObjectName("menu_view");
        menu_viewType = new QMenu(menu_view);
        menu_viewType->setObjectName("menu_viewType");
        MeltAway->setMenuBar(menubar);
        statusbar = new QStatusBar(MeltAway);
        statusbar->setObjectName("statusbar");
        MeltAway->setStatusBar(statusbar);

        menubar->addAction(menu_file->menuAction());
        menubar->addAction(menu_view->menuAction());
        menu_file->addSeparator();
        menu_file->addAction(action_exit);
        menu_view->addAction(menu_viewType->menuAction());
        menu_viewType->addAction(action_setViewType_solid);
        menu_viewType->addAction(action_setViewType_wire);

        retranslateUi(MeltAway);

        QMetaObject::connectSlotsByName(MeltAway);
    } // setupUi

    void retranslateUi(QMainWindow *MeltAway)
    {
        MeltAway->setWindowTitle(QCoreApplication::translate("MeltAway", "MeltAway", nullptr));
        action_exit->setText(QCoreApplication::translate("MeltAway", "\320\222\321\213\320\271\321\202\320\270", nullptr));
#if QT_CONFIG(tooltip)
        action_exit->setToolTip(QCoreApplication::translate("MeltAway", "\320\222\321\213\320\271\321\202\320\270 \320\270\320\267 \320\277\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\321\217", nullptr));
#endif // QT_CONFIG(tooltip)
        action_setViewType_solid->setText(QCoreApplication::translate("MeltAway", "\320\242\320\262\320\265\321\200\320\264\320\276\321\202\320\265\320\273\321\214\320\275\321\213\320\271", nullptr));
        action_setViewType_wire->setText(QCoreApplication::translate("MeltAway", "\320\241\320\265\321\202\320\276\321\207\320\275\321\213\320\271", nullptr));
        OpenButton->setText(QCoreApplication::translate("MeltAway", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        checkBox_BrushSelection->setText(QCoreApplication::translate("MeltAway", "Brush Selection", nullptr));
        menu_file->setTitle(QCoreApplication::translate("MeltAway", "\320\244\320\260\320\271\320\273", nullptr));
        menu_view->setTitle(QCoreApplication::translate("MeltAway", "\320\222\320\270\320\264", nullptr));
        menu_viewType->setTitle(QCoreApplication::translate("MeltAway", "\320\240\320\265\320\266\320\270\320\274 \320\276\321\202\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeltAway: public Ui_MeltAway {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MELTAWAY_H
