#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(
    LogNaturalNPCConversationGovernanceModule,
    Log,
    All
);

class FNaturalNPCConversationGovernanceModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        // Category policy lives in runtime components so this module stays lightweight.
        UE_LOG(
            LogNaturalNPCConversationGovernanceModule,
            Display,
            TEXT("Natural NPC Conversation Governance module started.")
        );
    }
};

IMPLEMENT_MODULE(
    FNaturalNPCConversationGovernanceModule,
    NaturalNPCConversationGovernance
)
