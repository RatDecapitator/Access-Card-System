#pragma once

#include <vector>

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




};