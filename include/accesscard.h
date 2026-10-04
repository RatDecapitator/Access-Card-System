#pragma once

#include <string>

#include "status.h"

#include <vector>

#include "cardhistory.h"

class AccessCard
{
    private:

        std::vector<CardHistory> history;
        std::string cardId;
        AccessCardStatus status;
        std::string ownerId;



    public:

        AccessCard(
            std::string accessCardId,
            std::string accessCardOwnerId,
            AccessCardStatus accessCardStatus
        );

        void setOwnerId(const std::string& newOwnerId );

        const std::string& getOwnerId() const;

        const std::string& getId() const;

        void setStatus(AccessCardStatus newStatus);

        AccessCardStatus getStatus() const;

        void addHistory(AccessCardStatus newStatus);

        const std::vector<CardHistory>& getHistory() const;

};


