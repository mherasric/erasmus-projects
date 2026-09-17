#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTableWidget;
class QListWidget;
class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddButtonClicked(); // Slot para el botón "Add"
    void onDeleteButtonClicked(); //Slot para el nuevo botón "delete"
    void onUpdateButtonClicked();
    void onFilterButtonClicked();
    void onPlayButtonClicked();
    void onNextButtonClicked();

private:
    QTableWidget *songTable;
    QListWidget *playlist;
    QLineEdit *titleEdit;
    QLineEdit *artistEdit;
    QLineEdit *durationEdit;
    QLineEdit *pathEdit;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *updateButton;
    QPushButton *filterButton;
    QPushButton *playButton;
    QPushButton *nextButton;
};
#endif
