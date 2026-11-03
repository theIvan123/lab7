#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_lab7.h"

class lab7 : public QMainWindow
{
    Q_OBJECT

public:
    lab7(QWidget *parent = nullptr);
    ~lab7();

private:
    Ui::lab7Class ui;
};

