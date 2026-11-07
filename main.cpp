#include <iostream>

#include "employee.h"
#include "accesscard.h"
#include "accesscontrolsystem.h"

int main()
{
    AccessControlSystem system;
    
    Employee employee1(
        "E000001",
        "Jhon Doe",
        EmployeeStatus::Working
    );

    AccessCard card1(
        "C000001",
        employee1.getId(),
        AccessCardStatus::Active
    );

    bool added_jhon_to_employee_base = system.addEmployee(employee1);

    if (added_jhon_to_employee_base)
    {
        std::cout << "New hire Jhon, has been added to employees database\n";
    }

    else
    {
        std::cout << "Coulnt add new hire to database";
    }

    bool added_new_card_for_jhon = system.addCard(card1);

    if (added_new_card_for_jhon)
    {
        std::cout << "Card for new hire has been added to the system\n";
    }
    else
    {
        std::cout << "Coldnt add new card for Jhon\n";
    }



    bool can_Jhon_access = system.canAccess("C000001");

    if (can_Jhon_access)
    {
        std::cout << "Access granted for Jhon Doe\n";
    }
    else
    {
        std::cout << "Access denied for Jhon Doe\n";
    }

    bool jhons_card_deactivation = system.deactivateCard("C000001");

    if (jhons_card_deactivation)
    {
        std::cout << "Jhons card has been deactivated\n";
    }

    else
    {
        std::cout << "Jhons card cannot be deactivated\n";
    }


    bool can_jhon_still_access = system.canAccess("C000001");
    
    if (can_jhon_still_access)
    {
        std::cout << "Access granted for Jhon Doe\n";
    }
    else
    {
        std::cout << "Access denied for Jhon Doe\n";
    }    


}