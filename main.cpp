#include <iostream>

#include "accesscontrolsystem.h"

namespace
{
    const char* employeeStatusName(EmployeeStatus status)
    {
        switch (status)
        {
            case EmployeeStatus::Working: return "Working";
            case EmployeeStatus::OnVacation: return "On vacation";
            case EmployeeStatus::Suspended: return "Suspended";
            case EmployeeStatus::Fired: return "Fired";
            case EmployeeStatus::SickLeave: return "Sick leave";
        }
        return "Unknown";
    }

    const char* cardStatusName(AccessCardStatus status)
    {
        switch (status)
        {
            case AccessCardStatus::Active: return "Active";
            case AccessCardStatus::Inactive: return "Inactive";
            case AccessCardStatus::Lost: return "Lost";
        }
        return "Unknown";
    }
}

int main()
{
    AccessControlSystem system;

    if (system.getEmployees().empty())
    {
        const auto employeeId = system.registerEmployee("Jhon Doe", EmployeeStatus::Working);
        if (!employeeId)
        {
            std::cerr << "Couldn't add the initial employee\n";
            return 1;
        }

        if (!system.registerCard(*employeeId, AccessCardStatus::Active))
        {
            std::cerr << "Couldn't add the initial card\n";
            return 1;
        }
    }

    std::cout << "Employees:\n";
    for (const auto& employee : system.getEmployees())
    {
        std::cout << "  " << employee.getId() << " - " << employee.getName()
                  << " (" << employeeStatusName(employee.getStatus()) << ")\n";
    }

    std::cout << "Cards:\n";
    for (const auto& card : system.getCards())
    {
        std::cout << "  " << card.getId() << " - owner " << card.getOwnerId()
                  << " (" << cardStatusName(card.getStatus()) << ")\n";
    }

    for (const auto& card : system.getCards())
    {
        std::cout << "Access for card " << card.getId() << ": "
                  << (system.canAccess(card.getId()) ? "granted" : "denied") << '\n';
    }
}