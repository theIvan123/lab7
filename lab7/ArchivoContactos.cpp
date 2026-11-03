#include "ArchivoContactos.h"
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QStringList>

QString ArchivoContactos::ruta() const
{
    return QDir::homePath() + "/Lab7Datos/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre,
    const QString& telefono,
    const QString& correo) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;

    QString linea = nombre + ";" + telefono + ";" + correo + "\n";
    QByteArray datos = linea.toUtf8();
    bool completo = archivo.write(datos) == datos.size();
    bool vaciado = archivo.flush();
    archivo.close();
    return completo && vaciado;
}

QVector<Contacto> ArchivoContactos::leer() const
{
    QVector<Contacto> contactos;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::ReadOnly))
        return contactos;

    while (!archivo.atEnd()) {
        QString linea = QString::fromUtf8(archivo.readLine()).trimmed();
        if (linea.isEmpty())
            continue;

        QStringList partes = linea.split(';');
        if (partes.size() < 3)
            continue;

        contactos.append({ partes[0], partes[1], partes[2] });
    }

    archivo.close();
    return contactos;
}

bool ArchivoContactos::actualizar(int indice, const Contacto& contacto) const
{
    QVector<Contacto> contactos = leer();
    if (indice < 0 || indice >= contactos.size())
        return false;

    contactos[indice] = contacto;
    return escribirTodos(contactos);
}

bool ArchivoContactos::eliminar(int indice) const
{
    QVector<Contacto> contactos = leer();
    if (indice < 0 || indice >= contactos.size())
        return false;

    contactos.removeAt(indice);
    return escribirTodos(contactos);
}

bool ArchivoContactos::escribirTodos(const QVector<Contacto>& contactos) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QByteArray datos;
    for (const Contacto& contacto : contactos) {
        datos += (contacto.nombre + ";" + contacto.telefono + ";" +
            contacto.correo + "\n").toUtf8();
    }

    bool completo = archivo.write(datos) == datos.size();
    bool vaciado = archivo.flush();
    archivo.close();
    return completo && vaciado;
}
