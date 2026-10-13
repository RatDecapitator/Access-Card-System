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
        "E000001",
        "Adam",
        EmployeeStatus::Working
    );

    Employee employee3(
        "E000002",
        "Adam",
        EmployeeStatus::Working
    );


    AccessCard card1(
        "C000001",
        employee1.getId(),
        AccessCardStatus::Lost
    );
 

    Employee employee4(
        "E99999",
        "Adam",
        EmployeeStatus::Working
    );
    
    AccessCard card4(
        "C000009",
        employee1.getId(),
        AccessCardStatus::Lost
    );


    AccessControlSystem system;

    system.addEmployee(employee1);
    system.addEmployee(employee2);
    system.addCard(card1);

    if (system.getCards()[0].getStatus() == AccessCardStatus::Lost)
    {
        std::cout << "Card 1 Lost" << "\n";
    }

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

    std::cout << system.canAccess("C000001") << "\n";

    bool changed = system.changeCardOwner("C000001", "E000002");

    if(changed)
    {
        std::cout << "Owner changed\n";
    }
    else
    {
        std::cout << "Owner change failed\n";
    }

    std::cout << "New owner: "
            << system.getCards()[0].getOwnerId()
            << "\n";    

    bool activated = system.activateCard("C000001");

    if(activated)
    {
        std::cout << "Card 1 activated\n";
    }
    else
    {
        std::cout << "Activation of card 1 failed\n";
    }




    bool deactivated = system.deactivateCard("C000001");

    if(deactivated)
    {
        std::cout << "Card 1 deactivated\n";
    }
    else
    {
        std::cout << "Deactivation of card 1  failed\n";
    }    


    for (const CardHistory& history : card1.getHistory())
    {
        if (history.newStatus == AccessCardStatus::Active)
        {
            std::cout << "Active\n";
        }
        else{
            std::cout << "Inactive\n";
        }
    }

    system.addEmployee(employee4);

    AccessCard card3(
        "C000002",
        employee3.getId(),
        AccessCardStatus::Active
    );

    system.addEmployee(employee3);
    system.addCard(card3);

    std::cout << "Cards: "
              << system.getCards().size()
              << "\n";
    


    
    if (card1.getStatus() == AccessCardStatus::Lost)
    {
        std::cout << "Lost - card 1" << "\n";
    }
    





    bool deactivated2 = system.deactivateCard("C000002");

    if(deactivated2)
    {
        std::cout << "Card 3 deactivated\n";
    }
    else
    {
        std::cout << "Deactivation of card 3  failed\n";
    }   

    bool activated2 = system.activateCard("C000002");

    if(activated2)
    {
        std::cout << "Card 3 activated\n";
    }
    else
    {
        std::cout << "Activation of card 3 failed\n";
    }

    bool deactivated3 = system.deactivateCard("C000009");

    if(deactivated3)
    {
        std::cout << "Card 4 deactivated\n";
    }
    else
    {
        std::cout << "Deactivation of card 4  failed\n";
    }   

    bool activated3 = system.activateCard("C000009");

    if(activated3)
    {
        std::cout << "Card 4 activated\n";
    }
    else
    {
        std::cout << "Activation of card 4 failed\n";
    }

    return 0;

}