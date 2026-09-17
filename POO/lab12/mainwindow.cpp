#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();

    // Configurar la tabla con 4 columnas (la última para el Path, que estará oculta)
    songTable = new QTableWidget(0, 4, this);
    songTable->setHorizontalHeaderLabels({"Title", "Artist", "Duration", "Path"});
    songTable->setColumnHidden(3, true); // Ocultar la columna "Path"
    leftLayout->addWidget(songTable);

    titleEdit = new QLineEdit(this);
    artistEdit = new QLineEdit(this);
    durationEdit = new QLineEdit(this);
    pathEdit = new QLineEdit(this);

    leftLayout->addWidget(new QLabel("Title:"));
    leftLayout->addWidget(titleEdit);
    leftLayout->addWidget(new QLabel("Artist:"));
    leftLayout->addWidget(artistEdit);
    leftLayout->addWidget(new QLabel("Duration:"));
    leftLayout->addWidget(durationEdit);
    leftLayout->addWidget(new QLabel("Path:"));
    leftLayout->addWidget(pathEdit);

    addButton = new QPushButton("Add", this);
    deleteButton = new QPushButton("Delete", this);
    updateButton = new QPushButton("Update", this);
    filterButton = new QPushButton("Filter", this);
    leftLayout->addWidget(addButton);
    leftLayout->addWidget(deleteButton);
    leftLayout->addWidget(updateButton);
    leftLayout->addWidget(filterButton);

    playlist = new QListWidget(this);
    rightLayout->addWidget(playlist);

    playButton = new QPushButton("Play", this);
    nextButton = new QPushButton("Next", this);
    rightLayout->addWidget(playButton);
    rightLayout->addWidget(nextButton);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);

    resize(600, 400);

    // Conectar el botón "Add" al slot
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateButtonClicked);
    connect(filterButton, &QPushButton::clicked, this, &MainWindow::onFilterButtonClicked);
    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayButtonClicked);
    connect(nextButton, &QPushButton:: clicked, this, &MainWindow::onNextButtonClicked);
}

void MainWindow::onAddButtonClicked()
{
    // Obtener los valores de los campos
    QString title = titleEdit->text();
    QString artist = artistEdit->text();
    QString duration = durationEdit->text();
    QString path = pathEdit->text();

    // Verificar que los campos no estén vacíos
    if (title.isEmpty() || artist.isEmpty() || duration.isEmpty() || path.isEmpty()) {
        return; // No agregar si algún campo está vacío
    }

    // Agregar una nueva fila a la tabla
    int row = songTable->rowCount();
    songTable->insertRow(row);

    // Llenar la fila con los datos
    songTable->setItem(row, 0, new QTableWidgetItem(title));
    songTable->setItem(row, 1, new QTableWidgetItem(artist));
    songTable->setItem(row, 2, new QTableWidgetItem(duration));
    songTable->setItem(row, 3, new QTableWidgetItem(path)); // Guardar el Path

    // Limpiar los campos después de agregar
    titleEdit->clear();
    artistEdit->clear();
    durationEdit->clear();
    pathEdit->clear();
}

void MainWindow::onDeleteButtonClicked()
{
    // Obtener la fila seleccionada
    int selectedRow = songTable->currentRow();

    // Verificar que haya una fila seleccionada
    if (selectedRow >= 0) {
        // Eliminar la fila seleccionada
        songTable->removeRow(selectedRow);
    }
}

void MainWindow::onUpdateButtonClicked()
{
    // Obtener la fila seleccionada
    int selectedRow = songTable->currentRow();

    // Verificar que haya una fila seleccionada
    if (selectedRow < 0) {
        return; // No hacer nada si no hay fila seleccionada
    }

    // Obtener los valores de los campos
    QString title = titleEdit->text();
    QString artist = artistEdit->text();
    QString duration = durationEdit->text();
    QString path = pathEdit->text();

    // Verificar que los campos no estén vacíos
    if (title.isEmpty() || artist.isEmpty() || duration.isEmpty() || path.isEmpty()) {
        return; // No actualizar si algún campo está vacío
    }

    // Actualizar la fila seleccionada
    songTable->setItem(selectedRow, 0, new QTableWidgetItem(title));
    songTable->setItem(selectedRow, 1, new QTableWidgetItem(artist));
    songTable->setItem(selectedRow, 2, new QTableWidgetItem(duration));
    songTable->setItem(selectedRow, 3, new QTableWidgetItem(path));

    // Limpiar los campos después de actualizar
    titleEdit->clear();
    artistEdit->clear();
    durationEdit->clear();
    pathEdit->clear();
}

void MainWindow::onFilterButtonClicked()
{
    QString filterText = titleEdit->text().toLower(); // Filtrar por título

    // Recorrer todas las filas de la tabla
    for (int row = 0; row < songTable->rowCount(); ++row) {
        QString title = songTable->item(row, 0)->text().toLower();
        bool match = title.contains(filterText);
        songTable->setRowHidden(row, !match); // Mostrar u ocultar la fila
    }
}

void MainWindow::onPlayButtonClicked()
{
    // Obtener la fila seleccionada en la tabla
    int selectedRow = songTable->currentRow();

    // Verificar que haya una fila seleccionada
    if (selectedRow < 0) {
        return; // No hacer nada si no hay fila seleccionada
    }

    // Obtener los datos de la canción
    QString title = songTable->item(selectedRow, 0)->text();
    QString artist = songTable->item(selectedRow, 1)->text();
    QString displayText = title + " - " + artist;

    // Agregar la canción a la lista de reproducción
    playlist->addItem(displayText);
}

void MainWindow::onNextButtonClicked()
{
    // Obtener el elemento actual en la lista de reproducción
    int currentRow = playlist->currentRow();

    // Verificar que haya elementos en la lista
    if (playlist->count() == 0) {
        return; // No hacer nada si la lista está vacía
    }

    // Avanzar al siguiente elemento (o volver al primero si estamos al final)
    int nextRow = (currentRow + 1) % playlist->count();
    playlist->setCurrentRow(nextRow);
}

MainWindow::~MainWindow()
{
}
