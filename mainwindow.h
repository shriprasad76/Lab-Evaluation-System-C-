#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

class QLineEdit;
class QTableWidget;
class QPushButton;

#include "student.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void addStudent();
    void calculateMarks();
    void updateStudent();
    void deleteStudent();
    void searchStudent();
    void tableRowSelected(int row, int column);

private:
    void setupUI();
    void loadStudents();
    void saveStudents();
    void displayStudents();
    void clearInputs();
    void fillInputs(const Student &student);
    Student readStudentFromInputs(bool &ok) const;
    int readMark(QLineEdit *editor, int maxValue, bool &ok) const;

    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QVector<QLineEdit *> practicalEdits;
    QLineEdit *viva1Edit;
    QLineEdit *viva2Edit;
    QLineEdit *viva3Edit;

    QPushButton *addButton;
    QPushButton *calculateButton;
    QPushButton *updateButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;

    QTableWidget *resultTable;
    QVector<Student> students;
};

#endif // MAINWINDOW_H
