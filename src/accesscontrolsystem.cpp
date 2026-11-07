#include "accesscontrolsystem.h"


bool AccessControlSystem::addEmployee(const Employee& employee)
{
    if (findEmployeeById(employee.getId()) == nullptr)
    {
        employees.push_back(employee);
        return true;
    }
    else
    {
        return false;
    }

}


bool AccessControlSystem::addCard(const AccessCard& card)
{
    std::string employeeId = card.getOwnerId();

    if (findCardById(card.getId()) == nullptr)
    {
        if (findEmployeeById(employeeId) != nullptr)
        {
            cards.push_back(card);
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
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
        if(id == employee.getId())
        {
            return &employee;
        }
    }

    return nullptr;
}


AccessCard* AccessControlSystem::findCardById(const std::string& id)
{
    for(auto& card : cards)
    {
        if(id == card.getId())
        {
            return &card;
        }
    }

    return nullptr;
}


bool AccessControlSystem::canAccess(const std::string& cardId)
{
    AccessCard* card = findCardById(cardId);

    if(card == nullptr)
    {
        return false;
    }


    if(card->getStatus() == AccessCardStatus::Active)
    {
        std::string personId = card->getOwnerId();
        Employee* person = findEmployeeById(personId);

        if(person == nullptr)
        {
            return false;
        }


        if(person->getStatus() == EmployeeStatus::Working)
        {
            return true;
        }
    }

    return false;
}


bool AccessControlSystem::changeCardOwner(const std::string& cardId, const std::string& newOwnerId)
{
    AccessCard* card = findCardById(cardId);

    if(card == nullptr)
    {
        return false;
    }


    Employee* employee = findEmployeeById(newOwnerId);

    if(employee == nullptr)
    {
        return false;
    }


    card->setOwnerId(newOwnerId);

    return true;
    
}

bool AccessControlSystem::activateCard(const std::string& cardId)
{
    AccessCard* card = findCardById(cardId);

    if(card == nullptr or card->getStatus() == AccessCardStatus::Lost)
    {
        return false;
    }

    card->setStatus(AccessCardStatus::Active);

    return true;
}

bool AccessControlSystem::deactivateCard(const std::string& cardId)
{
    AccessCard* card = findCardById(cardId);

    if(card == nullptr or card->getStatus() == AccessCardStatus::Lost )
    {
        return false;
    }

    card->setStatus(AccessCardStatus::Inactive);

    return true;
}

