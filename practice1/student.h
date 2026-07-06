#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

struct Student {
    QString surname;
    qint64 course;
    qint32 phone;
};

#endif // STUDENT_H