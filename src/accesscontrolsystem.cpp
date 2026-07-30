#include "accesscontrolsystem.h"


void AccessControlSystem::addEmployee(const Employee& employee)
{
    employees.push_back(employee);
}


void AccessControlSystem::addCard(const AccessCard& card)
{
    cards.push_back(card);
}

const std::vector<Employee>& AccessControlSystem::getEmployees() const
{
    return employees;
}

const std::vector<AccessCard>& AccessControlSystem::getCards() const
{
    return cards;
}

Employee* AccessControlSystem::findEmployeeById(const std::string& id)
{
    for(auto& employee : employees)
    {
        if (id == employee.getId())
        {
            return &employee;
        }
    }
    return nullptr;
}

AccessCard* AccessControlSystem::findCardById(const std::string& id)
{
    for(auto& card: cards)
    {
        if (id == card.getId())
        {
            return &card;
        }
    }
    return nullptr;
}