#include <iostream>

#include "employee.h"
#include "accesscard.h"
#include "accesscontrolsystem.h"

int main()
{
    Employee employee1(
        "E000001",
        "Oliwier",
        EmployeeStatus::Working
    );

    AccessCard card1(
        "C000001",
        employee1.getId(),
        AccessCardStatus::Active
    );

    AccessControlSystem system;

    system.addEmployee(employee1);
    system.addCard(card1);

    std::cout << "Employees: " << system.getEmployees().size() << "\n";
    std::cout << "Cards: " << system.getCards().size() << "\n";

    return 0;
}