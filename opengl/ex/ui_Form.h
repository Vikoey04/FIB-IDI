/********************************************************************************
** Form generated from reading UI file 'Form.ui'
**
** Created by: Qt User Interface Compiler version 5.12.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyForm
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *text_control;
    QSlider *h_slider;
    QHBoxLayout *horizontalLayout_2;
    QLabel *text_h;
    QSpinBox *h_spinbox;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *boto_sortir;

    void setupUi(QWidget *MyForm)
    {
        if (MyForm->objectName().isEmpty())
            MyForm->setObjectName(QString::fromUtf8("MyForm"));
        MyForm->resize(400, 300);
        verticalLayout_3 = new QVBoxLayout(MyForm);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        text_control = new QLabel(MyForm);
        text_control->setObjectName(QString::fromUtf8("text_control"));
        QFont font;
        font.setFamily(QString::fromUtf8("Noto Sans [GOOG]"));
        font.setPointSize(10);
        text_control->setFont(font);

        horizontalLayout->addWidget(text_control);

        h_slider = new QSlider(MyForm);
        h_slider->setObjectName(QString::fromUtf8("h_slider"));
        h_slider->setMinimum(130);
        h_slider->setMaximum(230);
        h_slider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(h_slider);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        text_h = new QLabel(MyForm);
        text_h->setObjectName(QString::fromUtf8("text_h"));
        text_h->setFont(font);

        horizontalLayout_2->addWidget(text_h);

        h_spinbox = new QSpinBox(MyForm);
        h_spinbox->setObjectName(QString::fromUtf8("h_spinbox"));
        h_spinbox->setMinimum(130);
        h_spinbox->setMaximum(230);

        horizontalLayout_2->addWidget(h_spinbox);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        boto_sortir = new QPushButton(MyForm);
        boto_sortir->setObjectName(QString::fromUtf8("boto_sortir"));

        horizontalLayout_3->addWidget(boto_sortir);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(MyForm);
        QObject::connect(h_slider, SIGNAL(valueChanged(int)), h_spinbox, SLOT(setValue(int)));
        QObject::connect(h_spinbox, SIGNAL(valueChanged(int)), h_slider, SLOT(setValue(int)));
        QObject::connect(boto_sortir, SIGNAL(clicked()), MyForm, SLOT(close()));

        QMetaObject::connectSlotsByName(MyForm);
    } // setupUi

    void retranslateUi(QWidget *MyForm)
    {
        MyForm->setWindowTitle(QApplication::translate("MyForm", "Form", nullptr));
        text_control->setText(QApplication::translate("MyForm", "Control d'al\303\247ada", nullptr));
        text_h->setText(QApplication::translate("MyForm", "Al\303\247ada en metres", nullptr));
        boto_sortir->setText(QApplication::translate("MyForm", "&Sortir", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyForm: public Ui_MyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
