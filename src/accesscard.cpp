#include "accesscard.h"

AccessCard::AccessCard(
    std::string accessCardId,
    std::string accessCardOwnerId,
    AccessCardStatus accessCardStatus
)

   : cardId(accessCardId),
     ownerId(accessCardOwnerId),
     status(accessCardStatus)

{

}

void AccessCard::setOwnerId(const std::string& newOwnerId)
{
   ownerId = newOwnerId;
}

const std::string& AccessCard::getOwnerId() const
{
   return ownerId;
}

void AccessCard::setStatus(AccessCardStatus newStatus)
{
    addHistory(newStatus);
    status = newStatus;

}

void AccessCard::addHistory(AccessCardStatus newStatus)
{
    CardHistory newHistory;
    newHistory.newStatus = newStatus;

    history.push_back(newHistory);


}

AccessCardStatus AccessCard::getStatus() const
{
    return status;
}

const std::string& AccessCard::getId() const
{
    return cardId;
}


const std::vector<CardHistory>& AccessCard::getHistory() const
{
    return history;
}






