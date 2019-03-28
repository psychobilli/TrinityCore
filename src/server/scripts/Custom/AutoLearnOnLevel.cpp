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

struct AutoLearnSpellReqs
{
    AutoLearnSpellInfo SpellInfo;
    std::list<uint32> ReqSpell;
};

class AutoLearnOnLevel : PlayerScript
{
public:
    AutoLearnOnLevel() : PlayerScript("AutoLearnOnLevel") {}

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        uint8 currLevel = player->getLevel();
        std::list<AutoLearnSpellInfo> infoList = GetNpcSpellListing(player->getClass(), player->getRace(), oldLevel, currLevel);
        std::list<AutoLearnSpellReqs> reqsList = GetRequiredSpells(infoList);

        for (uint32 level = oldLevel + 1; level <= currLevel; level++) {
            for (AutoLearnSpellReqs reqs : reqsList) {
                if (reqs.SpellInfo.ReqLevel == level) {
                    if (reqs.ReqSpell.empty()) {
                        player->LearnSpell(reqs.SpellInfo.SpellID, false, false);
                    }
                    else {
                        uint8 spells = reqs.ReqSpell.size();
                        uint8 spellCount = 0;
                        for (uint32 reqSpell : reqs.ReqSpell) {
                            if (player->HasSpell(reqSpell)) {
                                spellCount += 1;
                            }
                        }
                        if (spells == spellCount) {
                            player->LearnSpell(reqs.SpellInfo.SpellID, false, false);
                        }
                    }
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
        std::list<AutoLearnSpellInfo> infoList;
        if (trainerReference.size() > 0) {
            QueryResult result = WorldDatabase.PQuery("SELECT SpellId, ReqSkillLine, ReqLevel FROM npc_trainer WHERE ID IN (%s) AND ReqLevel BETWEEN %u AND %u", trainerReference, (oldLevel + 1), currentLevel);

            if (result)
            {
                do
                {
                    Field* fields = result->Fetch();
                    AutoLearnSpellInfo info;
                    info.SpellID = fields[0].GetInt32();
                    info.ReqSkillLine = fields[1].GetInt32();
                    info.ReqLevel = fields[2].GetInt32();

                    infoList.push_back(info);
                } while (result->NextRow());
            }
        }
        return infoList;
    }

    std::list<AutoLearnSpellReqs> GetRequiredSpells(std::list<AutoLearnSpellInfo> infoList) {
        std::string spellIds = "";
        for (AutoLearnSpellInfo info : infoList) {
            if (spellIds.length() > 0) {
                spellIds += ",";
            }
            spellIds += std::to_string(info.SpellID);
        }
        std::list<AutoLearnSpellReqs> reqsList;
        if (spellIds.length() > 0)
        {
            QueryResult result = WorldDatabase.PQuery("select first_spell_id, spell_id from spell_ranks where spell_id in (%s) union select req_spell, spell_id from spell_required where spell_id in (%s) order by spell_id", spellIds, spellIds);
            if (result)
            {
                AutoLearnSpellReqs spellReqs;
                spellReqs.SpellInfo.SpellID = 0;
                uint32 spellId = 0;
                uint32 reqSpell = 0;
                do
                {
                    Field* fields = result->Fetch();
                    spellId = fields[1].GetInt32();

                    if (spellReqs.SpellInfo.SpellID != spellId) {
                        if (spellReqs.SpellInfo.SpellID > 0) {
                            reqsList.push_back(spellReqs);
                            spellReqs = AutoLearnSpellReqs();
                        }
                        for (AutoLearnSpellInfo info : infoList) {
                            if (info.SpellID == spellId) {
                                spellReqs.SpellInfo = info;
                                break;
                            }
                        }
                    }

                    reqSpell = fields[0].GetInt32();
                    if (reqSpell > 0 && reqSpell != spellId) {
                        spellReqs.ReqSpell.push_back(reqSpell);
                    }
                } while (result->NextRow());

                if (reqSpell > 0 && reqSpell != spellId) {
                    spellReqs.ReqSpell.push_back(reqSpell);
                }
                reqsList.push_back(spellReqs);
            }
        }
        for (AutoLearnSpellInfo info : infoList) {
            bool noReqs = true;
            for (AutoLearnSpellReqs reqs : reqsList) {
                if (info.SpellID == reqs.SpellInfo.SpellID) {
                    noReqs = false;
                }
            }
            if (noReqs) {
                AutoLearnSpellReqs spellReq;
                spellReq.SpellInfo = info;
                reqsList.push_back(spellReq);
            }
        }
        return reqsList;
    }
};

void AddSC_AutoLearnOnLevel()
{
    new AutoLearnOnLevel();
}
