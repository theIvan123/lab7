#pragma once
#include <QString>
#include <QVector>

struct Contacto
{
    QString nombre;
    QString telefono;
    QString correo;
};

class ArchivoContactos
{
public:
    QString ruta() const;
    bool guardar(const QString& nombre,
        const QString& telefono,
        const QString& correo) const;
    QVector<Contacto> leer() const;
    bool actualizar(int indice, const Contacto& contacto) const;
    bool eliminar(int indice) const;

private:
    bool escribirTodos(const QVector<Contacto>& contactos) const;
};
