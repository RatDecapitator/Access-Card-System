#pragma once

#include <string>

enum class AccessCardStatus
{
    Active,
    Inactive,
    Lost,
    Broken
};

class AccessCard
{
    private:

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

};


