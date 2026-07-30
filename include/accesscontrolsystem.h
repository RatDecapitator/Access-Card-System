#pragma once

#include <vector>

#include <string>

#include "employee.h"

#include "accesscard.h"


class AccessControlSystem
{
    private:

        std::vector<Employee> employees;
        std::vector<AccessCard> cards;

    public:

        void addEmployee(const Employee& employee);
        
        void addCard(const AccessCard& card);
        
        const std::vector<Employee>& getEmployees() const;

        const std::vector<AccessCard>& getCards() const;

        Employee* findEmployeeById(const std::string& id);

        AccessCard* findCardById(const std::string& id);

        bool canAccess(const std::string& cardId);

        bool changeCardOwner(const std::string& cardId, const std::string& newOwnerId);

        bool activateCard(const std::string& cardId);

        bool deactivateCard(const std::string& cardId);

};