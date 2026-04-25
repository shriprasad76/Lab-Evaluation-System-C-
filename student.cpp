#include "student.h"
#include <QStringList>
#include <QtGlobal>

Student::Student(const QString &studentId,
                 const QString &studentName,
                 const QVector<int> &practicals,
                 int vivaOne,
                 int vivaTwo,
                 int vivaThree)
    : id(studentId), name(studentName), practicalMarks(practicals), viva1(vivaOne), viva2(vivaTwo), viva3(vivaThree)
{
    calculateTotals();
}

void Student::calculateTotals()
{
    if (practicalMarks.size() != 10) {
        practicalMarks.resize(10);
    }

    practicalTotal = 0;
    for (int i = 0; i < practicalMarks.size(); ++i) {
        practicalMarks[i] = qBound(0, practicalMarks[i], 10);
        practicalTotal += practicalMarks[i];
    }

    viva1 = qBound(0, viva1, 15);
    viva2 = qBound(0, viva2, 15);
    viva3 = qBound(0, viva3, 30);

    vivaTotal = viva1 + viva2 + viva3;
    grandTotal = practicalTotal + vivaTotal;
    finalOutOf50 = (grandTotal / 160.0) * 50.0;
}

QString Student::toLine() const
{
    QStringList fields;
    fields << id << name;
    for (int mark : practicalMarks) {
        fields << QString::number(mark);
    }
    fields << QString::number(viva1) << QString::number(viva2) << QString::number(viva3);
    return fields.join("|");
}

Student Student::fromLine(const QString &line)
{
    Student student;
    QStringList fields = line.split('|');
    if (fields.size() == 15) {
        student.id = fields[0].trimmed();
        student.name = fields[1].trimmed();
        student.practicalMarks.resize(10);
        for (int i = 0; i < 10; ++i) {
            student.practicalMarks[i] = fields[2 + i].toInt();
        }
        student.viva1 = fields[12].toInt();
        student.viva2 = fields[13].toInt();
        student.viva3 = fields[14].toInt();
    }
    student.calculateTotals();
    return student;
}
