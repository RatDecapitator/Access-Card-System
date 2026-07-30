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

    Employee employee2(
        "E000002",
        "Adam",
        EmployeeStatus::Working
    );


    AccessCard card1(
        "C000001",
        employee1.getId(),
        AccessCardStatus::Active
    );


    AccessControlSystem system;

    system.addEmployee(employee1);
    system.addEmployee(employee2);
    system.addCard(card1);


    std::cout << "Employees: "
              << system.getEmployees().size()
              << "\n";

    std::cout << "Cards: "
              << system.getCards().size()
              << "\n";


    Employee* foundEmployee = system.findEmployeeById("E000002");

    if(foundEmployee)
    {
        std::cout << "Found employee: "
                  << foundEmployee->getName()
                  << "\n";
    }
    else
    {
        std::cout << "Employee not found\n";
    }



    AccessCard* foundCard = system.findCardById("C000001");

    if(foundCard)
    {
        std::cout << "Found card: "
                << foundCard->getId()
                << "\n";
    }
    else
    {
        std::cout << "Card not found\n";
    }    

    return 0;
}