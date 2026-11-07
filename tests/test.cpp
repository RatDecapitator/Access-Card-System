#include <gtest/gtest.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

#include "employee.h"
#include "accesscard.h"
#include "accesscontrolsystem.h"

namespace
{
    std::filesystem::path makePersistenceTestPath(const std::string& name)
    {
        const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
        return std::filesystem::temp_directory_path()
            / ("access-card-" + name + "-" + std::to_string(timestamp) + ".db");
    }

    struct TemporaryAccessControlSystem
    {
        std::filesystem::path path;
        AccessControlSystem system;

        TemporaryAccessControlSystem()
            : path(makePersistenceTestPath("unit")),
              system(path.string())
        {
        }

        ~TemporaryAccessControlSystem()
        {
            std::error_code error;
            std::filesystem::remove(path, error);
        }
    };
}

TEST(employee_test, new_employee_added)
{
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;

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
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;

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
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;
    
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
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;
    
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
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;

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
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;

    const auto found_employee = system.findEmployeeById("E999999");

    EXPECT_FALSE(found_employee);
}


// FIND CARD TESTS

TEST(card_test, find_existing_card)
{
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;

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
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;

    const auto found_card = system.findCardById("C999999");

    EXPECT_FALSE(found_card);
}

TEST(employee_test, register_employee_generates_sequential_ids)
{
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;

    const auto first = system.registerEmployee("First Worker", EmployeeStatus::Working);
    const auto second = system.registerEmployee("Second Worker", EmployeeStatus::Working);

    ASSERT_TRUE(first);
    ASSERT_TRUE(second);
    EXPECT_EQ(*first, "E000001");
    EXPECT_EQ(*second, "E000002");
}

TEST(employee_test, register_employee_advances_past_existing_ids)
{
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;
    system.addEmployee(Employee("E000004", "Existing Worker", EmployeeStatus::Working));

    const auto added = system.registerEmployee("New Worker", EmployeeStatus::Working);

    ASSERT_TRUE(added);
    EXPECT_EQ(*added, "E000005");
}

TEST(card_test, register_card_generates_sequential_ids)
{
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;
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
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;
    system.addEmployee(Employee("E000001", "Worker", EmployeeStatus::Working));
    system.addCard(AccessCard("C000008", "E000001", AccessCardStatus::Active));

    const auto added = system.registerCard("E000001", AccessCardStatus::Active);

    ASSERT_TRUE(added);
    EXPECT_EQ(*added, "C000009");
}

TEST(card_test, register_card_rejects_unknown_owner)
{
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;

    EXPECT_FALSE(system.registerCard("E000001", AccessCardStatus::Active));
}

TEST(employee_test, register_employee_fails_after_maximum_id)
{
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;
    system.addEmployee(Employee("E999999", "Last Worker", EmployeeStatus::Working));

    EXPECT_FALSE(system.registerEmployee("Extra Worker", EmployeeStatus::Working));
}

TEST(card_test, register_card_fails_after_maximum_id)
{
    TemporaryAccessControlSystem temporarySystem;
    auto& system = temporarySystem.system;
    system.addEmployee(Employee("E000001", "Worker", EmployeeStatus::Working));
    system.addCard(AccessCard("C999999", "E000001", AccessCardStatus::Active));

    EXPECT_FALSE(system.registerCard("E000001", AccessCardStatus::Active));
}

TEST(persistence_test, restores_records_history_and_next_ids_after_restart)
{
    const auto path = makePersistenceTestPath("restart");
    std::string firstEmployeeId;
    std::string firstCardId;

    {
        AccessControlSystem system(path.string());
        const auto employeeId = system.registerEmployee("Jane \"JJ\"\nDoe", EmployeeStatus::Working);
        ASSERT_TRUE(employeeId);
        firstEmployeeId = *employeeId;

        const auto secondEmployeeId = system.registerEmployee("John\nDoe", EmployeeStatus::Working);
        ASSERT_TRUE(secondEmployeeId);

        const auto cardId = system.registerCard(firstEmployeeId, AccessCardStatus::Active);
        ASSERT_TRUE(cardId);
        firstCardId = *cardId;
        ASSERT_TRUE(system.changeCardOwner(firstCardId, *secondEmployeeId));
        ASSERT_TRUE(system.deactivateCard(firstCardId));

        const auto firstEmployee = system.findEmployeeById(firstEmployeeId);
        ASSERT_TRUE(firstEmployee);
        firstEmployee->get().setName("Updated Employee");
        firstEmployee->get().setStatus(EmployeeStatus::OnVacation);
        system.save();
    }

    {
        AccessControlSystem restored(path.string());
        ASSERT_EQ(restored.getEmployees().size(), 2);
        ASSERT_EQ(restored.getCards().size(), 1);

        const auto employee = restored.findEmployeeById(firstEmployeeId);
        ASSERT_TRUE(employee);
        EXPECT_EQ(employee->get().getName(), "Updated Employee");
        EXPECT_EQ(employee->get().getStatus(), EmployeeStatus::OnVacation);
        const auto secondEmployee = restored.findEmployeeById("E000002");
        ASSERT_TRUE(secondEmployee);
        EXPECT_EQ(secondEmployee->get().getName(), "John\nDoe");

        const auto card = restored.findCardById(firstCardId);
        ASSERT_TRUE(card);
        EXPECT_EQ(card->get().getOwnerId(), "E000002");
        EXPECT_EQ(card->get().getStatus(), AccessCardStatus::Inactive);
        ASSERT_EQ(card->get().getHistory().size(), 1);
        EXPECT_EQ(card->get().getHistory().front().newStatus, AccessCardStatus::Inactive);

        const auto nextEmployee = restored.registerEmployee("After Restart", EmployeeStatus::Working);
        const auto nextCard = restored.registerCard(firstEmployeeId, AccessCardStatus::Active);
        ASSERT_TRUE(nextEmployee);
        ASSERT_TRUE(nextCard);
        EXPECT_EQ(*nextEmployee, "E000003");
        EXPECT_EQ(*nextCard, "C000002");
    }

    std::filesystem::remove(path);
}

TEST(persistence_test, rejects_corrupt_file_without_overwriting_it)
{
    const auto path = makePersistenceTestPath("corrupt");
    const std::string contents = "not a valid access card database\n";
    {
        std::ofstream output(path);
        ASSERT_TRUE(output);
        output << contents;
    }

    EXPECT_THROW(AccessControlSystem(path.string()), std::runtime_error);

    std::ifstream input(path);
    const std::string preserved(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>()
    );
    EXPECT_EQ(preserved, contents);
    std::filesystem::remove(path);
}

TEST(persistence_test, failed_save_rolls_back_in_memory_addition)
{
    const auto path = makePersistenceTestPath("missing-parent") / "records.db";
    AccessControlSystem system(path.string());

    EXPECT_THROW(
        system.addEmployee(Employee("E000001", "Unsaved", EmployeeStatus::Working)),
        std::runtime_error
    );
    EXPECT_TRUE(system.getEmployees().empty());
    EXPECT_FALSE(system.findEmployeeById("E000001"));
}