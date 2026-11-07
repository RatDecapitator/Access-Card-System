#include <iostream>

#include "employee.h"
#include "accesscard.h"
#include "accesscontrolsystem.h"

int main()
{
    AccessControlSystem system;

    const auto employee = system.registerEmployee("Jhon Doe", EmployeeStatus::Working);
    if (employee)
    {
        std::cout << "New hire Jhon Doe (" << *employee
                  << ") has been added to employees database\n";
    }
    else
    {
        std::cout << "Couldn't add new hire to database\n";
        return 1;
    }

    const auto card = system.registerCard(*employee, AccessCardStatus::Active);
    if (card)
    {
        std::cout << "Card " << *card << " for new hire has been added to the system\n";
    }
    else
    {
        std::cout << "Couldn't add new card for Jhon\n";
        return 1;
    }

    const bool can_Jhon_access = system.canAccess(*card);

    if (can_Jhon_access)
    {
        std::cout << "Access granted for Jhon Doe\n";
    }
    else
    {
        std::cout << "Access denied for Jhon Doe\n";
    }

    const bool jhons_card_deactivation = system.deactivateCard(*card);

    if (jhons_card_deactivation)
    {
        std::cout << "Jhons card has been deactivated\n";
    }

    else
    {
        std::cout << "Jhons card cannot be deactivated\n";
    }


    const bool can_jhon_still_access = system.canAccess(*card);
    
    if (can_jhon_still_access)
    {
        std::cout << "Access granted for Jhon Doe\n";
    }
    else
    {
        std::cout << "Access denied for Jhon Doe\n";
    }    


}