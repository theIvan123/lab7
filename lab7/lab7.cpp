#include "lab7.h"
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAction>

lab7::lab7(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.tablaContactos->horizontalHeader()->setStretchLastSection(true);

    connect(ui.btnGuardar, &QPushButton::clicked,
        this, &lab7::guardarContacto);
    connect(ui.actionRegistrar, &QAction::triggered,
        this, &lab7::mostrarRegistrar);
    connect(ui.actionHistorial, &QAction::triggered,
        this, [this] { mostrarContactos(ModoContactos::Historial); });
    connect(ui.actionEditar, &QAction::triggered,
        this, [this] { mostrarContactos(ModoContactos::Editar); });
    connect(ui.actionEliminar, &QAction::triggered,
        this, [this] { mostrarContactos(ModoContactos::Eliminar); });
    connect(ui.btnActualizar, &QPushButton::clicked,
        this, &lab7::actualizarContacto);
    connect(ui.btnEliminar, &QPushButton::clicked,
        this, &lab7::eliminarContacto);
    connect(ui.tablaContactos, &QTableWidget::itemSelectionChanged,
        this, &lab7::cargarSeleccion);

    mostrarRegistrar();
}

lab7::~lab7()
{
}

bool lab7::validarDatos(const QString& nombre,
    const QString& telefono,
    const QString& correo)
{
    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos",
            "Completá los tres campos.");
        return false;
    }

    for (const QString& dato : QStringList{ nombre, telefono, correo }) {
        if (dato.contains(';') || dato.contains('\n') ||
            dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido",
                "No usés punto y coma ni saltos de línea.");
            return false;
        }
    }

    return true;
}

void lab7::guardarContacto()
{
    QString nombre = ui.txtNombre->text().trimmed();
    QString telefono = ui.txtTelefono->text().trimmed();
    QString correo = ui.txtCorreo->text().trimmed();

    if (!validarDatos(nombre, telefono, correo))
        return;

    if (!archivoContactos.guardar(nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo guardar",
            "Revisá permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    ui.txtNombre->clear();
    ui.txtTelefono->clear();
    ui.txtCorreo->clear();
    QMessageBox::information(this, "Guardado completo",
        "El contacto se guardó correctamente.\nArchivo: "
        + archivoContactos.ruta());
    ui.txtNombre->setFocus();
}

void lab7::mostrarRegistrar()
{
    ui.stackedWidget->setCurrentWidget(ui.pageRegistrar);
    ui.txtNombre->setFocus();
}

void lab7::mostrarContactos(ModoContactos modo)
{
    modoActual = modo;
    cargarContactos();
    limpiarEdicion();

    bool permiteEditar = (modo == ModoContactos::Editar);
    ui.txtEditNombre->setEnabled(permiteEditar);
    ui.txtEditTelefono->setEnabled(permiteEditar);
    ui.txtEditCorreo->setEnabled(permiteEditar);
    ui.btnActualizar->setEnabled(permiteEditar);
    ui.btnEliminar->setEnabled(modo == ModoContactos::Eliminar);

    switch (modo) {
    case ModoContactos::Historial:
        ui.lblTituloContactos->setText("Historial de contactos");
        break;
    case ModoContactos::Editar:
        ui.lblTituloContactos->setText(
            "Editar contacto (seleccione una fila)");
        break;
    case ModoContactos::Eliminar:
        ui.lblTituloContactos->setText(
            "Eliminar contacto (seleccione una fila)");
        break;
    }

    ui.stackedWidget->setCurrentWidget(ui.pageContactos);
}

void lab7::cargarContactos()
{
    const QVector<Contacto> contactos = archivoContactos.leer();

    ui.tablaContactos->clearContents();
    ui.tablaContactos->setRowCount(contactos.size());

    for (int fila = 0; fila < contactos.size(); ++fila) {
        const Contacto& contacto = contactos[fila];
        ui.tablaContactos->setItem(fila, 0,
            new QTableWidgetItem(contacto.nombre));
        ui.tablaContactos->setItem(fila, 1,
            new QTableWidgetItem(contacto.telefono));
        ui.tablaContactos->setItem(fila, 2,
            new QTableWidgetItem(contacto.correo));
    }
}

void lab7::cargarSeleccion()
{
    int fila = ui.tablaContactos->currentRow();
    if (fila < 0) {
        limpiarEdicion();
        return;
    }

    QTableWidgetItem* itemNombre = ui.tablaContactos->item(fila, 0);
    QTableWidgetItem* itemTelefono = ui.tablaContactos->item(fila, 1);
    QTableWidgetItem* itemCorreo = ui.tablaContactos->item(fila, 2);
    if (!itemNombre || !itemTelefono || !itemCorreo)
        return;

    ui.txtEditNombre->setText(itemNombre->text());
    ui.txtEditTelefono->setText(itemTelefono->text());
    ui.txtEditCorreo->setText(itemCorreo->text());
}

void lab7::actualizarContacto()
{
    int fila = ui.tablaContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Sin selección",
            "Seleccione el contacto que desea editar.");
        return;
    }

    Contacto contacto;
    contacto.nombre = ui.txtEditNombre->text().trimmed();
    contacto.telefono = ui.txtEditTelefono->text().trimmed();
    contacto.correo = ui.txtEditCorreo->text().trimmed();

    if (!validarDatos(contacto.nombre, contacto.telefono, contacto.correo))
        return;

    if (!archivoContactos.actualizar(fila, contacto)) {
        QMessageBox::critical(this, "No se pudo actualizar",
            "Revisá permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    QMessageBox::information(this, "Contacto actualizado",
        "Los datos se actualizaron correctamente.");
    cargarContactos();
    limpiarEdicion();
}

void lab7::eliminarContacto()
{
    int fila = ui.tablaContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Sin selección",
            "Seleccione el contacto que desea eliminar.");
        return;
    }

    QTableWidgetItem* itemNombre = ui.tablaContactos->item(fila, 0);
    QString nombre = itemNombre ? itemNombre->text() : QString();

    QMessageBox::StandardButton respuesta = QMessageBox::question(
        this, "Confirmar eliminación",
        "¿Eliminar el contacto \"" + nombre + "\"?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (respuesta != QMessageBox::Yes)
        return;

    if (!archivoContactos.eliminar(fila)) {
        QMessageBox::critical(this, "No se pudo eliminar",
            "Revisá permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    QMessageBox::information(this, "Contacto eliminado",
        "El contacto se eliminó correctamente.");
    cargarContactos();
    limpiarEdicion();
}

void lab7::limpiarEdicion()
{
    ui.txtEditNombre->clear();
    ui.txtEditTelefono->clear();
    ui.txtEditCorreo->clear();
    ui.tablaContactos->clearSelection();
}
