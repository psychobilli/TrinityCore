#include <string>
#include "DatabaseEnv.h"
#include "Define.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "Player.h"

struct AutoLearnSpellInfo
{
    uint32 SpellID;
    uint32 ReqSkillLine;
    uint32 ReqLevel;
};

class AutoLearnOnLevel : PlayerScript
{
public:
    AutoLearnOnLevel() : PlayerScript("AutoLearnOnLevel") {}

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        uint8 currLevel = player->getLevel();
        std::list<AutoLearnSpellInfo> alsiList = GetNpcSpellListing(player->getClass(), player->getRace(), oldLevel, currLevel);

        for (uint32 level = oldLevel + 1; level <= currLevel; level++) {
            for (AutoLearnSpellInfo alsi : alsiList) {
                if (alsi.ReqLevel == level) {
                    bool dependent = false;
                    if (alsi.ReqSkillLine > 0) {
                        dependent = true;
                    }
                    player->LearnSpell(alsi.SpellID, dependent, false);
                }
            }
        }
    }

private:
    std::list<AutoLearnSpellInfo> GetNpcSpellListing(uint8 playerClass, uint8 playerRace, uint8 oldLevel, uint8 currentLevel) {
        std::string trainerReference = "";
        switch (playerClass) {
        case CLASS_DEATH_KNIGHT:
            trainerReference = "200019";
            break;
        case CLASS_DRUID:
            trainerReference = "200005,200006";
            break;
        case CLASS_HUNTER:
            trainerReference = "200013,200014";
            break;
        case CLASS_MAGE:
            trainerReference = "200007,200008";
            break;
        case CLASS_PALADIN:
            if (playerRace == RACE_HUMAN || playerRace == RACE_NIGHTELF || playerRace == RACE_DWARF || playerRace == RACE_GNOME || playerRace == RACE_DRAENEI) {
                trainerReference = "200003,200004,200020";
            }
            else {
                trainerReference = "200003,200004,200021";
            }
            break;
        case CLASS_PRIEST:
            trainerReference = "200011,200012";
            break;
        case CLASS_ROGUE:
            trainerReference = "200015,200016";
            break;
        case CLASS_SHAMAN:
            trainerReference = "200017,200018";
            break;
        case CLASS_WARLOCK:
            trainerReference = "200009,200010";
            break;
        case CLASS_WARRIOR:
            trainerReference = "200001,200002";
            break;
        }
        std::list<AutoLearnSpellInfo> alsiList;
        if (trainerReference.size() > 0) {
            QueryResult result = WorldDatabase.PQuery("SELECT SpellId, ReqSkillLine, ReqLevel FROM npc_trainer WHERE ID IN (%s) AND ReqLevel BETWEEN %u AND %u", trainerReference, (oldLevel + 1), currentLevel);

            if (result)
            {
                do
                {
                    Field* fields = result->Fetch();
                    AutoLearnSpellInfo alsi;
                    alsi.SpellID = fields[0].GetInt32();
                    alsi.ReqSkillLine = fields[1].GetInt32();
                    alsi.ReqLevel = fields[2].GetInt32();

                    alsiList.push_back(alsi);
                } while (result->NextRow());
            }
        }
        return alsiList;
    }
};

void AddSC_AutoLearnOnLevel()
{
    new AutoLearnOnLevel();
}
