#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "employee.h"
#include "accesscard.h"


class AccessControlSystem
{
    private:

        std::vector<Employee> employees;
        std::vector<AccessCard> cards;
        std::unordered_map<std::string, std::size_t> employeeIndexes;
        std::unordered_map<std::string, std::size_t> cardIndexes;
        unsigned int nextEmployeeSequence = 1;
        unsigned int nextCardSequence = 1;

        static std::optional<unsigned int> parseSequence(const std::string& id, char prefix);
        static std::string formatId(char prefix, unsigned int sequence);
        static void advanceSequence(unsigned int& nextSequence, const std::string& id, char prefix);

    public:

        bool addEmployee(const Employee& employee);
        
        bool addCard(const AccessCard& card);

        std::optional<std::string> registerEmployee(
            const std::string& name,
            EmployeeStatus status
        );

        std::optional<std::string> registerCard(
            const std::string& ownerId,
            AccessCardStatus status
        );
        
        const std::vector<Employee>& getEmployees() const;

        const std::vector<AccessCard>& getCards() const;

        std::optional<std::reference_wrapper<Employee>> findEmployeeById(const std::string& id);

        std::optional<std::reference_wrapper<const Employee>> findEmployeeById(const std::string& id) const;

        std::optional<std::reference_wrapper<AccessCard>> findCardById(const std::string& id);

        std::optional<std::reference_wrapper<const AccessCard>> findCardById(const std::string& id) const;

        bool canAccess(const std::string& cardId);

        bool changeCardOwner(const std::string& cardId, const std::string& newOwnerId);

        bool activateCard(const std::string& cardId);

        bool deactivateCard(const std::string& cardId);

};