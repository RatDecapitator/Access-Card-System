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

    const auto found_employee = system.findEmployeeById("E000001");

    ASSERT_TRUE(found_employee);
    EXPECT_EQ(found_employee->get().getId(), "E000001");
}

TEST(employee_test, find_non_existing_employee)
{
    AccessControlSystem system;

    const auto found_employee = system.findEmployeeById("E999999");

    EXPECT_FALSE(found_employee);
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

    const auto found_card = system.findCardById("C000001");

    ASSERT_TRUE(found_card);
    EXPECT_EQ(found_card->get().getId(), "C000001");
}

TEST(card_test, find_non_existing_card)
{
    AccessControlSystem system;

    const auto found_card = system.findCardById("C999999");

    EXPECT_FALSE(found_card);
}

TEST(employee_test, register_employee_generates_sequential_ids)
{
    AccessControlSystem system;

    const auto first = system.registerEmployee("First Worker", EmployeeStatus::Working);
    const auto second = system.registerEmployee("Second Worker", EmployeeStatus::Working);

    ASSERT_TRUE(first);
    ASSERT_TRUE(second);
    EXPECT_EQ(*first, "E000001");
    EXPECT_EQ(*second, "E000002");
}

TEST(employee_test, register_employee_advances_past_existing_ids)
{
    AccessControlSystem system;
    system.addEmployee(Employee("E000004", "Existing Worker", EmployeeStatus::Working));

    const auto added = system.registerEmployee("New Worker", EmployeeStatus::Working);

    ASSERT_TRUE(added);
    EXPECT_EQ(*added, "E000005");
}

TEST(card_test, register_card_generates_sequential_ids)
{
    AccessControlSystem system;
    const auto employee = system.registerEmployee("Worker", EmployeeStatus::Working);
    ASSERT_TRUE(employee);

    const auto first = system.registerCard(*employee, AccessCardStatus::Active);
    const auto second = system.registerCard(*employee, AccessCardStatus::Active);

    ASSERT_TRUE(first);
    ASSERT_TRUE(second);
    EXPECT_EQ(*first, "C000001");
    EXPECT_EQ(*second, "C000002");
}

TEST(card_test, register_card_advances_past_existing_ids)
{
    AccessControlSystem system;
    system.addEmployee(Employee("E000001", "Worker", EmployeeStatus::Working));
    system.addCard(AccessCard("C000008", "E000001", AccessCardStatus::Active));

    const auto added = system.registerCard("E000001", AccessCardStatus::Active);

    ASSERT_TRUE(added);
    EXPECT_EQ(*added, "C000009");
}

TEST(card_test, register_card_rejects_unknown_owner)
{
    AccessControlSystem system;

    EXPECT_FALSE(system.registerCard("E000001", AccessCardStatus::Active));
}

TEST(employee_test, register_employee_fails_after_maximum_id)
{
    AccessControlSystem system;
    system.addEmployee(Employee("E999999", "Last Worker", EmployeeStatus::Working));

    EXPECT_FALSE(system.registerEmployee("Extra Worker", EmployeeStatus::Working));
}

TEST(card_test, register_card_fails_after_maximum_id)
{
    AccessControlSystem system;
    system.addEmployee(Employee("E000001", "Worker", EmployeeStatus::Working));
    system.addCard(AccessCard("C999999", "E000001", AccessCardStatus::Active));

    EXPECT_FALSE(system.registerCard("E000001", AccessCardStatus::Active));
}