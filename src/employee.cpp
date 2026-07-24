#include "employee.h"

Employee::Employee(
    std::string employeeId,
    std::string employeeName,
    EmployeeStatus employeeStatus
)
    : id(employeeId),
    name(employeeName),
    status(employeeStatus)

{

}


void Employee::setName(const std::string& newName)
{
    name = newName;
}

const std::string& Employee::getName() const
{
    return name;
}

void Employee::setStatus(EmployeeStatus newEmployeeStatus)
{
    status = newEmployeeStatus;
}

EmployeeStatus Employee::getStatus() const
{
    return status;
}

const std::string& Employee::getId() const
{
    return id;
}