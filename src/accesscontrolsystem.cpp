#include "accesscontrolsystem.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace
{
    constexpr std::size_t maximumPersistedRecords = 1000000;

    bool isValidEmployeeStatus(int status)
    {
        return status >= static_cast<int>(EmployeeStatus::Working)
            && status <= static_cast<int>(EmployeeStatus::SickLeave);
    }

    bool isValidCardStatus(int status)
    {
        return status >= static_cast<int>(AccessCardStatus::Active)
            && status <= static_cast<int>(AccessCardStatus::Lost);
    }
}


AccessControlSystem::AccessControlSystem(std::string persistenceFile)
    : persistencePath(std::move(persistenceFile))
{
    if (persistencePath.empty())
    {
        throw std::invalid_argument("Persistence file path cannot be empty");
    }

    load();
}


void AccessControlSystem::load()
{
    std::error_code fileError;
    const bool fileExists = std::filesystem::exists(persistencePath, fileError);
    if (fileError)
    {
        throw std::runtime_error("Unable to inspect persistence file: " + persistencePath);
    }
    if (!fileExists)
    {
        return;
    }

    std::ifstream input(persistencePath);
    if (!input)
    {
        throw std::runtime_error("Unable to open persistence file: " + persistencePath);
    }

    std::string fileSignature;
    unsigned int fileVersion = 0;
    if (!(input >> fileSignature >> fileVersion)
        || fileSignature != "ACCESS_CARD_SYSTEM"
        || fileVersion != 1)
    {
        throw std::runtime_error("Invalid or unsupported persistence file: " + persistencePath);
    }

    std::string section;
    std::size_t employeeCount = 0;
    if (!(input >> section >> employeeCount)
        || section != "EMPLOYEES"
        || employeeCount > maximumPersistedRecords)
    {
        throw std::runtime_error("Invalid employee section in persistence file: " + persistencePath);
    }

    std::vector<Employee> loadedEmployees;
    std::unordered_map<std::string, std::size_t> loadedEmployeeIndexes;
    unsigned int loadedNextEmployeeSequence = 1;
    loadedEmployees.reserve(employeeCount);
    loadedEmployeeIndexes.reserve(employeeCount);

    for (std::size_t index = 0; index < employeeCount; ++index)
    {
        std::string recordType;
        std::string id;
        std::string name;
        int status = 0;
        if (!(input >> recordType >> std::quoted(id) >> status >> std::quoted(name))
            || recordType != "EMPLOYEE"
            || !isValidEmployeeStatus(status)
            || !loadedEmployeeIndexes.emplace(id, loadedEmployees.size()).second)
        {
            throw std::runtime_error("Invalid employee record in persistence file: " + persistencePath);
        }

        loadedEmployees.emplace_back(
            id,
            name,
            static_cast<EmployeeStatus>(status)
        );
        advanceSequence(loadedNextEmployeeSequence, id, 'E');
    }

    std::size_t cardCount = 0;
    if (!(input >> section >> cardCount)
        || section != "CARDS"
        || cardCount > maximumPersistedRecords)
    {
        throw std::runtime_error("Invalid card section in persistence file: " + persistencePath);
    }

    std::vector<AccessCard> loadedCards;
    std::unordered_map<std::string, std::size_t> loadedCardIndexes;
    unsigned int loadedNextCardSequence = 1;
    loadedCards.reserve(cardCount);
    loadedCardIndexes.reserve(cardCount);

    for (std::size_t index = 0; index < cardCount; ++index)
    {
        std::string recordType;
        std::string id;
        std::string ownerId;
        int status = 0;
        std::size_t historyCount = 0;
        if (!(input >> recordType >> std::quoted(id) >> std::quoted(ownerId) >> status >> historyCount)
            || recordType != "CARD"
            || !isValidCardStatus(status)
            || historyCount > maximumPersistedRecords
            || loadedEmployeeIndexes.find(ownerId) == loadedEmployeeIndexes.end()
            || !loadedCardIndexes.emplace(id, loadedCards.size()).second)
        {
            throw std::runtime_error("Invalid card record in persistence file: " + persistencePath);
        }

        AccessCard card(id, ownerId, static_cast<AccessCardStatus>(status));
        for (std::size_t historyIndex = 0; historyIndex < historyCount; ++historyIndex)
        {
            std::string historyType;
            int historyStatus = 0;
            if (!(input >> historyType >> historyStatus)
                || historyType != "HISTORY"
                || !isValidCardStatus(historyStatus))
            {
                throw std::runtime_error("Invalid card history in persistence file: " + persistencePath);
            }
            card.addHistory(static_cast<AccessCardStatus>(historyStatus));
        }

        loadedCards.push_back(std::move(card));
        advanceSequence(loadedNextCardSequence, id, 'C');
    }

    input >> std::ws;
    if (input.peek() != std::char_traits<char>::eof())
    {
        throw std::runtime_error("Unexpected data in persistence file: " + persistencePath);
    }

    employees = std::move(loadedEmployees);
    employeeIndexes = std::move(loadedEmployeeIndexes);
    cards = std::move(loadedCards);
    cardIndexes = std::move(loadedCardIndexes);
    nextEmployeeSequence = loadedNextEmployeeSequence;
    nextCardSequence = loadedNextCardSequence;
}


void AccessControlSystem::save() const
{
    const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const std::string temporaryPath = persistencePath + ".tmp." + std::to_string(timestamp);

    try
    {
        std::ofstream output(temporaryPath, std::ios::out | std::ios::trunc);
        if (!output)
        {
            throw std::runtime_error("Unable to create temporary persistence file: " + temporaryPath);
        }

        output << "ACCESS_CARD_SYSTEM 1\n";
        output << "EMPLOYEES " << employees.size() << '\n';
        for (const auto& employee : employees)
        {
            output << "EMPLOYEE "
                   << std::quoted(employee.getId()) << ' '
                   << static_cast<int>(employee.getStatus()) << ' '
                   << std::quoted(employee.getName()) << '\n';
        }

        output << "CARDS " << cards.size() << '\n';
        for (const auto& card : cards)
        {
            output << "CARD "
                   << std::quoted(card.getId()) << ' '
                   << std::quoted(card.getOwnerId()) << ' '
                   << static_cast<int>(card.getStatus()) << ' '
                   << card.getHistory().size();
            for (const auto& history : card.getHistory())
            {
                output << " HISTORY " << static_cast<int>(history.newStatus);
            }
            output << '\n';
        }

        output.flush();
        if (!output)
        {
            throw std::runtime_error("Unable to write persistence file: " + temporaryPath);
        }
        output.close();
        if (!output)
        {
            throw std::runtime_error("Unable to close persistence file: " + temporaryPath);
        }

        std::error_code renameError;
        std::filesystem::rename(temporaryPath, persistencePath, renameError);
        if (renameError)
        {
            throw std::runtime_error("Unable to replace persistence file: " + persistencePath);
        }
    }
    catch (...)
    {
        std::error_code cleanupError;
        std::filesystem::remove(temporaryPath, cleanupError);
        throw;
    }
}

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
        const auto insertion = employeeIndexes.emplace(employee.getId(), employees.size() - 1);
        if (!insertion.second)
        {
            employees.pop_back();
            return false;
        }
        save();
    }
    catch (...)
    {
        employeeIndexes.erase(employee.getId());
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
        const auto insertion = cardIndexes.emplace(card.getId(), cards.size() - 1);
        if (!insertion.second)
        {
            cards.pop_back();
            return false;
        }
        save();
    }
    catch (...)
    {
        cardIndexes.erase(card.getId());
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

    const std::string previousOwnerId = card->get().getOwnerId();
    card->get().setOwnerId(newOwnerId);
    try
    {
        save();
    }
    catch (...)
    {
        card->get().setOwnerId(previousOwnerId);
        throw;
    }
    return true;
}

bool AccessControlSystem::activateCard(const std::string& cardId)
{
    const auto card = findCardById(cardId);
    if (!card || card->get().getStatus() == AccessCardStatus::Lost)
    {
        return false;
    }

    const AccessCard previousCard = card->get();
    card->get().setStatus(AccessCardStatus::Active);
    try
    {
        save();
    }
    catch (...)
    {
        card->get() = previousCard;
        throw;
    }
    return true;
}

bool AccessControlSystem::deactivateCard(const std::string& cardId)
{
    const auto card = findCardById(cardId);
    if (!card || card->get().getStatus() == AccessCardStatus::Lost)
    {
        return false;
    }

    const AccessCard previousCard = card->get();
    card->get().setStatus(AccessCardStatus::Inactive);
    try
    {
        save();
    }
    catch (...)
    {
        card->get() = previousCard;
        throw;
    }
    return true;
}
