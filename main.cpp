#include <iostream>
#include "employee.h"


int main()
{
    Employee employee(
        "E000001",
        "Oliwier",
        EmployeeStatus::Working
    );


    std::cout << employee.getName() << std::endl;

    employee.setName("Adam");

    std::cout << employee.getName() << std::endl;
}