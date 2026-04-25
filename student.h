#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QVector>

class Student
{
public:
    Student() = default;
    Student(const QString &studentId,
            const QString &studentName,
            const QVector<int> &practicals,
            int vivaOne,
            int vivaTwo,
            int vivaThree);

    QString id;
    QString name;
    QVector<int> practicalMarks;
    int viva1{0};
    int viva2{0};
    int viva3{0};

    int practicalTotal{0};
    int vivaTotal{0};
    int grandTotal{0};
    double finalOutOf50{0.0};

    void calculateTotals();
    QString toLine() const;
    static Student fromLine(const QString &line);
};

#endif // STUDENT_H
