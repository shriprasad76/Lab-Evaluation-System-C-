#include "mainwindow.h"

#include <QFile>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTextStream>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), idEdit(new QLineEdit(this)), nameEdit(new QLineEdit(this)), viva1Edit(new QLineEdit(this)), viva2Edit(new QLineEdit(this)), viva3Edit(new QLineEdit(this)), resultTable(new QTableWidget(this))
{
    setupUI();
    loadStudents();
    displayStudents();
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Student details area
    QGroupBox *studentGroup = new QGroupBox(tr("Student Details"), central);
    QFormLayout *studentForm = new QFormLayout(studentGroup);
    studentForm->addRow(tr("Student ID:"), idEdit);
    studentForm->addRow(tr("Student Name:"), nameEdit);
    studentGroup->setLayout(studentForm);
    mainLayout->addWidget(studentGroup);

    // Practicals section
    QGroupBox *practicalGroup = new QGroupBox(tr("Practical Marks (10 x 10)"), central);
    QGridLayout *practicalLayout = new QGridLayout(practicalGroup);
    practicalEdits.reserve(10);
    for (int i = 0; i < 10; ++i) {
        QLabel *label = new QLabel(tr("P%1:").arg(i + 1), practicalGroup);
        QLineEdit *editor = new QLineEdit(practicalGroup);
        editor->setPlaceholderText(tr("0-10"));
        practicalEdits.append(editor);
        practicalLayout->addWidget(label, i / 5, (i % 5) * 2);
        practicalLayout->addWidget(editor, i / 5, (i % 5) * 2 + 1);
    }
    practicalGroup->setLayout(practicalLayout);
    mainLayout->addWidget(practicalGroup);

    // Viva section
    QGroupBox *vivaGroup = new QGroupBox(tr("Viva Marks"), central);
    QFormLayout *vivaForm = new QFormLayout(vivaGroup);
    viva1Edit->setPlaceholderText(tr("0-15"));
    viva2Edit->setPlaceholderText(tr("0-15"));
    viva3Edit->setPlaceholderText(tr("0-30"));
    vivaForm->addRow(tr("Viva 1:"), viva1Edit);
    vivaForm->addRow(tr("Viva 2:"), viva2Edit);
    vivaForm->addRow(tr("Viva 3:"), viva3Edit);
    vivaGroup->setLayout(vivaForm);
    mainLayout->addWidget(vivaGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    calculateButton = new QPushButton(tr("Calculate"), central);
    addButton = new QPushButton(tr("Add Student"), central);
    updateButton = new QPushButton(tr("Update Student"), central);
    deleteButton = new QPushButton(tr("Delete Student"), central);
    searchButton = new QPushButton(tr("Search by ID"), central);
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(searchButton);
    mainLayout->addLayout(buttonLayout);

    // Result table
    resultTable->setColumnCount(19);
    resultTable->setHorizontalHeaderLabels({
        tr("ID"), tr("Name"), tr("P Total"), tr("Viva Total"), tr("Grand Total"), tr("Final /50"),
        tr("P1"), tr("P2"), tr("P3"), tr("P4"), tr("P5"),
        tr("P6"), tr("P7"), tr("P8"), tr("P9"), tr("P10"),
        tr("V1"), tr("V2"), tr("V3")
    });

    resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    resultTable->setSelectionBehavior(QTableWidget::SelectRows);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(resultTable, &QTableWidget::cellClicked, this, &MainWindow::tableRowSelected);

    mainLayout->addWidget(resultTable);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::calculateMarks);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateStudent);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteStudent);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchStudent);
}

void MainWindow::loadStudents()
{
    QFile file("students.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            Student student = Student::fromLine(line);
            if (!student.id.isEmpty()) {
                students.append(student);
            }
        }
    }
}

void MainWindow::saveStudents()
{
    QFile file("students.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Save error"), tr("Unable to open students.txt for writing."));
        return;
    }

    QTextStream out(&file);
    for (const Student &student : students) {
        out << student.toLine() << '\n';
    }
}

void MainWindow::displayStudents()
{
    resultTable->setRowCount(students.size());
    resultTable->setColumnCount(19);
    resultTable->setHorizontalHeaderLabels({
        tr("ID"), tr("Name"), tr("P Total"), tr("Viva Total"), tr("Grand Total"), tr("Final /50"),
        tr("P1"), tr("P2"), tr("P3"), tr("P4"), tr("P5"),
        tr("P6"), tr("P7"), tr("P8"), tr("P9"), tr("P10"),
        tr("V1"), tr("V2"), tr("V3")
    });

    for (int row = 0; row < students.size(); ++row) {
        const Student &student = students[row];
        resultTable->setItem(row, 0, new QTableWidgetItem(student.id));
        resultTable->setItem(row, 1, new QTableWidgetItem(student.name));
        resultTable->setItem(row, 2, new QTableWidgetItem(QString::number(student.practicalTotal)));
        resultTable->setItem(row, 3, new QTableWidgetItem(QString::number(student.vivaTotal)));
        resultTable->setItem(row, 4, new QTableWidgetItem(QString::number(student.grandTotal)));
        resultTable->setItem(row, 5, new QTableWidgetItem(QString::number(student.finalOutOf50, 'f', 2)));
        for (int i = 0; i < 10; ++i) {
            resultTable->setItem(row, 6 + i, new QTableWidgetItem(QString::number(student.practicalMarks.value(i, 0))));
        }
        resultTable->setItem(row, 16, new QTableWidgetItem(QString::number(student.viva1)));
        resultTable->setItem(row, 17, new QTableWidgetItem(QString::number(student.viva2)));
        resultTable->setItem(row, 18, new QTableWidgetItem(QString::number(student.viva3)));
    }
    resultTable->resizeColumnsToContents();
}

void MainWindow::clearInputs()
{
    idEdit->clear();
    nameEdit->clear();
    for (QLineEdit *edit : practicalEdits) {
        edit->clear();
    }
    viva1Edit->clear();
    viva2Edit->clear();
    viva3Edit->clear();
}

void MainWindow::fillInputs(const Student &student)
{
    idEdit->setText(student.id);
    nameEdit->setText(student.name);
    for (int i = 0; i < practicalEdits.size(); ++i) {
        practicalEdits[i]->setText(QString::number(student.practicalMarks.value(i, 0)));
    }
    viva1Edit->setText(QString::number(student.viva1));
    viva2Edit->setText(QString::number(student.viva2));
    viva3Edit->setText(QString::number(student.viva3));
}

Student MainWindow::readStudentFromInputs(bool &ok) const
{
    ok = false;
    const QString id = idEdit->text().trimmed();
    const QString name = nameEdit->text().trimmed();
    if (id.isEmpty()) {
        QMessageBox::warning(this, tr("Validation error"), tr("Student ID cannot be empty."));
        return Student();
    }
    if (name.isEmpty()) {
        QMessageBox::warning(this, tr("Validation error"), tr("Student Name cannot be empty."));
        return Student();
    }

    QVector<int> practicals;
    practicals.reserve(10);
    for (QLineEdit *editor : practicalEdits) {
        int mark = readMark(editor, 10, ok);
        if (!ok) {
            return Student();
        }
        practicals.append(mark);
    }

    const int v1 = readMark(viva1Edit, 15, ok);
    if (!ok) {
        return Student();
    }
    const int v2 = readMark(viva2Edit, 15, ok);
    if (!ok) {
        return Student();
    }
    const int v3 = readMark(viva3Edit, 30, ok);
    if (!ok) {
        return Student();
    }

    ok = true;
    return Student(id, name, practicals, v1, v2, v3);
}

int MainWindow::readMark(QLineEdit *editor, int maxValue, bool &ok) const
{
    const QString text = editor->text().trimmed();
    bool valid = false;
    const int value = text.toInt(&valid);
    if (!valid || value < 0 || value > maxValue) {
        QMessageBox::warning(this, tr("Validation error"), tr("Each mark must be a number between 0 and %1.").arg(maxValue));
        ok = false;
        return 0;
    }
    ok = true;
    return value;
}

void MainWindow::addStudent()
{
    bool ok = false;
    const Student student = readStudentFromInputs(ok);
    if (!ok) {
        return;
    }

    for (const Student &existing : students) {
        if (existing.id == student.id) {
            QMessageBox::warning(this, tr("Duplicate ID"), tr("A student with this ID already exists."));
            return;
        }
    }

    students.append(student);
    saveStudents();
    displayStudents();
    clearInputs();
}

void MainWindow::calculateMarks()
{
    bool ok = false;
    const Student student = readStudentFromInputs(ok);
    if (!ok) {
        return;
    }

    QMessageBox::information(this,
                             tr("Calculation Result"),
                             tr("Practical Total: %1 /100\nViva Total: %2 /60\nGrand Total: %3 /160\nFinal Marks: %4 /50")
                                 .arg(student.practicalTotal)
                                 .arg(student.vivaTotal)
                                 .arg(student.grandTotal)
                                 .arg(QString::number(student.finalOutOf50, 'f', 2)));
}

void MainWindow::updateStudent()
{
    bool ok = false;
    const Student student = readStudentFromInputs(ok);
    if (!ok) {
        return;
    }

    for (int index = 0; index < students.size(); ++index) {
        if (students[index].id == student.id) {
            students[index] = student;
            saveStudents();
            displayStudents();
            QMessageBox::information(this, tr("Updated"), tr("Student record updated successfully."));
            return;
        }
    }

    QMessageBox::warning(this, tr("Not found"), tr("Student ID not found for update."));
}

void MainWindow::deleteStudent()
{
    const QString id = idEdit->text().trimmed();
    if (id.isEmpty()) {
        QMessageBox::warning(this, tr("Validation error"), tr("Enter an ID to delete."));
        return;
    }

    for (int index = 0; index < students.size(); ++index) {
        if (students[index].id == id) {
            students.removeAt(index);
            saveStudents();
            displayStudents();
            clearInputs();
            QMessageBox::information(this, tr("Deleted"), tr("Student record deleted."));
            return;
        }
    }

    QMessageBox::warning(this, tr("Not found"), tr("Student ID not found for deletion."));
}

void MainWindow::searchStudent()
{
    const QString id = idEdit->text().trimmed();
    if (id.isEmpty()) {
        QMessageBox::warning(this, tr("Validation error"), tr("Enter an ID to search."));
        return;
    }

    for (int row = 0; row < students.size(); ++row) {
        if (students[row].id == id) {
            fillInputs(students[row]);
            resultTable->selectRow(row);
            QMessageBox::information(this, tr("Found"), tr("Student record loaded into input fields."));
            return;
        }
    }

    QMessageBox::warning(this, tr("Not found"), tr("Student ID not found."));
}

void MainWindow::tableRowSelected(int row, int /*column*/)
{
    if (row < 0 || row >= students.size()) {
        return;
    }
    fillInputs(students[row]);
}
