#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_lab7.h"
#include "ArchivoContactos.h"

class lab7 : public QMainWindow
{
    Q_OBJECT

public:
    lab7(QWidget *parent = nullptr);
    ~lab7();

private:
    enum class ModoContactos
    {
        Historial,
        Editar,
        Eliminar
    };

    Ui::lab7Class ui;
    ArchivoContactos archivoContactos;
    ModoContactos modoActual = ModoContactos::Historial;

    void guardarContacto();
    void mostrarRegistrar();
    void mostrarContactos(ModoContactos modo);
    void cargarContactos();
    void cargarSeleccion();
    void actualizarContacto();
    void eliminarContacto();
    void limpiarEdicion();
    bool validarDatos(const QString& nombre,
        const QString& telefono,
        const QString& correo);
};
