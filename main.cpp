#include <iostream>

#include "employee.h"
#include "accesscard.h"


int main()
{
    // Tworzymy pracownika
    Employee employee1(
        "E000001",
        "Oliwier",
        EmployeeStatus::Working
    );


    // Tworzymy kartę przypisaną do pracownika
    AccessCard card1(
        "C000001",
        employee1.getId(),
        AccessCardStatus::Active
    );


    // Sprawdzamy dane pracownika
    std::cout << "Employee ID: "
              << employee1.getId()
              << "\n";

    std::cout << "Employee name: "
              << employee1.getName()
              << "\n";


    // Sprawdzamy dane karty
    std::cout << "Card ID: "
              << card1.getId()
              << "\n";

    std::cout << "Card owner ID: "
              << card1.getOwnerId()
              << "\n";


    // Zmieniamy status karty
    card1.setStatus(AccessCardStatus::Lost);


    // Sprawdzamy czy zmiana działa
    if(card1.getStatus() == AccessCardStatus::Lost)
    {
        std::cout << "Card status: Lost\n";
    }


    return 0;
}