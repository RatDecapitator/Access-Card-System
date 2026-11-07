#include "accesscontrolsystem.h"

#include <iomanip>
#include <sstream>


std::optional<unsigned int> AccessControlSystem::parseSequence(const std::string& id, char prefix)
{
    if (id.size() != 7 || id.front() != prefix)
    {
        return std::nullopt;
    }

    unsigned int sequence = 0;
    for (std::size_t index = 1; index < id.size(); ++index)
    {
        if (id[index] < '0' || id[index] > '9')
        {
            return std::nullopt;
        }

        sequence = sequence * 10 + static_cast<unsigned int>(id[index] - '0');
    }

    return sequence;
}


std::string AccessControlSystem::formatId(char prefix, unsigned int sequence)
{
    std::ostringstream id;
    id << prefix << std::setw(6) << std::setfill('0') << sequence;
    return id.str();
}


void AccessControlSystem::advanceSequence(unsigned int& nextSequence, const std::string& id, char prefix)
{
    const auto sequence = parseSequence(id, prefix);
    if (sequence && *sequence >= nextSequence)
    {
        nextSequence = *sequence + 1;
    }
}


bool AccessControlSystem::addEmployee(const Employee& employee)
{
    if (employeeIndexes.find(employee.getId()) != employeeIndexes.end())
    {
        return false;
    }

    employees.push_back(employee);
    try
    {
        employeeIndexes.emplace(employee.getId(), employees.size() - 1);
    }
    catch (...)
    {
        employees.pop_back();
        throw;
    }

    advanceSequence(nextEmployeeSequence, employee.getId(), 'E');
    return true;
}


bool AccessControlSystem::addCard(const AccessCard& card)
{
    if (cardIndexes.find(card.getId()) != cardIndexes.end())
    {
        return false;
    }

    if (employeeIndexes.find(card.getOwnerId()) == employeeIndexes.end())
    {
        return false;
    }

    cards.push_back(card);
    try
    {
        cardIndexes.emplace(card.getId(), cards.size() - 1);
    }
    catch (...)
    {
        cards.pop_back();
        throw;
    }

    advanceSequence(nextCardSequence, card.getId(), 'C');
    return true;
}


std::optional<std::string> AccessControlSystem::registerEmployee(
    const std::string& name,
    EmployeeStatus status
)
{
    if (nextEmployeeSequence > 999999)
    {
        return std::nullopt;
    }

    const std::string id = formatId('E', nextEmployeeSequence);
    if (!addEmployee(Employee(id, name, status)))
    {
        return std::nullopt;
    }

    return id;
}


std::optional<std::string> AccessControlSystem::registerCard(
    const std::string& ownerId,
    AccessCardStatus status
)
{
    if (employeeIndexes.find(ownerId) == employeeIndexes.end() || nextCardSequence > 999999)
    {
        return std::nullopt;
    }

    const std::string id = formatId('C', nextCardSequence);
    if (!addCard(AccessCard(id, ownerId, status)))
    {
        return std::nullopt;
    }

    return id;
}


const std::vector<Employee>& AccessControlSystem::getEmployees() const
{
    return employees;
}


const std::vector<AccessCard>& AccessControlSystem::getCards() const
{
    return cards;
}


std::optional<std::reference_wrapper<Employee>> AccessControlSystem::findEmployeeById(const std::string& id)
{
    const auto index = employeeIndexes.find(id);
    if (index == employeeIndexes.end())
    {
        return std::nullopt;
    }

    return employees[index->second];
}


std::optional<std::reference_wrapper<const Employee>> AccessControlSystem::findEmployeeById(const std::string& id) const
{
    const auto index = employeeIndexes.find(id);
    if (index == employeeIndexes.end())
    {
        return std::nullopt;
    }

    return employees[index->second];
}


std::optional<std::reference_wrapper<AccessCard>> AccessControlSystem::findCardById(const std::string& id)
{
    const auto index = cardIndexes.find(id);
    if (index == cardIndexes.end())
    {
        return std::nullopt;
    }

    return cards[index->second];
}


std::optional<std::reference_wrapper<const AccessCard>> AccessControlSystem::findCardById(const std::string& id) const
{
    const auto index = cardIndexes.find(id);
    if (index == cardIndexes.end())
    {
        return std::nullopt;
    }

    return cards[index->second];
}


bool AccessControlSystem::canAccess(const std::string& cardId)
{
    const auto card = findCardById(cardId);
    if (!card)
    {
        return false;
    }

    if (card->get().getStatus() == AccessCardStatus::Active)
    {
        const auto person = findEmployeeById(card->get().getOwnerId());
        if (!person)
        {
            return false;
        }

        if (person->get().getStatus() == EmployeeStatus::Working)
        {
            return true;
        }
    }

    return false;
}


bool AccessControlSystem::changeCardOwner(const std::string& cardId, const std::string& newOwnerId)
{
    const auto card = findCardById(cardId);
    if (!card)
    {
        return false;
    }

    if (!findEmployeeById(newOwnerId))
    {
        return false;
    }

    card->get().setOwnerId(newOwnerId);
    return true;
}

bool AccessControlSystem::activateCard(const std::string& cardId)
{
    const auto card = findCardById(cardId);
    if (!card || card->get().getStatus() == AccessCardStatus::Lost)
    {
        return false;
    }

    card->get().setStatus(AccessCardStatus::Active);
    return true;
}

bool AccessControlSystem::deactivateCard(const std::string& cardId)
{
    const auto card = findCardById(cardId);
    if (!card || card->get().getStatus() == AccessCardStatus::Lost)
    {
        return false;
    }

    card->get().setStatus(AccessCardStatus::Inactive);
    return true;
}
