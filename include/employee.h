#pragma once

#include <string>

#include "status.h"

class Employee

{
    private:

        std::string id ;
        EmployeeStatus status;
        std::string name;
    
    
    public:

        Employee(
            std::string employeeId,
            std::string employeeName,
            EmployeeStatus employeeStatus
        );
       

        void setName(const std::string& newName);

        const std::string& getName() const;

        void setStatus(EmployeeStatus newStatus);

        EmployeeStatus getStatus() const;

        const std::string& getId() const;

};