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
    status = newStatus;
}

AccessCardStatus AccessCard::getStatus() const
{
    return status;
}

const std::string& AccessCard::getId() const
{
    return cardId;
}

