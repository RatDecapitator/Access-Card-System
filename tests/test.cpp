#include <gtest/gtest.h>
#include "employee.h"
#include "accesscard.h"
#include "accesscontrolsystem.h"

TEST(employee_test, new_employee_added)
{
    AccessControlSystem system;

    Employee employee2(
        "E000002",
        "Jane Doe",
        EmployeeStatus::Working
    );

    bool added_jane_to_employee_base = system.addEmployee(employee2);

    EXPECT_TRUE(added_jane_to_employee_base);
}

TEST(employee_test, add_duplicate_employee)
{
    AccessControlSystem system;

    Employee employee2(
        "E000002",
        "Jane Doe",
        EmployeeStatus::Working
    );

    Employee employee3(
        "E000002",
        "Janice Doe",
        EmployeeStatus::Working
    );

    system.addEmployee(employee2);

    bool added_janice_to_employee_base = system.addEmployee(employee3);

    EXPECT_FALSE(added_janice_to_employee_base);
}



TEST(card_test, add_new_card)
{
    AccessControlSystem system;
    
     Employee employee2(
        "E000002",
        "Jane Doe",
        EmployeeStatus::Working
    );

    system.addEmployee(employee2);

    AccessCard card2(
        "C000002",
        employee2.getId(),
        AccessCardStatus::Active
    );

    bool added_new_card = system.addCard(card2);

    EXPECT_TRUE(added_new_card);
}

TEST(card_test, add_new_card_negative)
{
    AccessControlSystem system;
    
    AccessCard card2(
        "C000002",
        "E000000",
        AccessCardStatus::Active
    );

    bool added_new_card = system.addCard(card2);

    EXPECT_FALSE(added_new_card);
}

// FIND EMPLOYEE TESTS

TEST(employee_test, find_existing_employee)
{
    AccessControlSystem system;

    Employee employee(
        "E000001",
        "John Doe",
        EmployeeStatus::Working
    );

    system.addEmployee(employee);

    Employee* found_employee = system.findEmployeeById("E000001");

    ASSERT_NE(found_employee, nullptr);
    EXPECT_EQ(found_employee->getId(), "E000001");
}

TEST(employee_test, find_non_existing_employee)
{
    AccessControlSystem system;

    Employee* found_employee = system.findEmployeeById("E999999");

    EXPECT_EQ(found_employee, nullptr);
}


// FIND CARD TESTS

TEST(card_test, find_existing_card)
{
    AccessControlSystem system;

    Employee employee(
        "E000001",
        "John Doe",
        EmployeeStatus::Working
    );

    system.addEmployee(employee);

    AccessCard card(
        "C000001",
        employee.getId(),
        AccessCardStatus::Active
    );

    system.addCard(card);

    AccessCard* found_card = system.findCardById("C000001");

    ASSERT_NE(found_card, nullptr);
    EXPECT_EQ(found_card->getId(), "C000001");
}

TEST(card_test, find_non_existing_card)
{
    AccessControlSystem system;

    AccessCard* found_card = system.findCardById("C999999");

    EXPECT_EQ(found_card, nullptr);
}