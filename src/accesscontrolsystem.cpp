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