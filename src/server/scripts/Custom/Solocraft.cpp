#include <map>

#include "Config.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Player.h"
#include "Pet.h"
#include "Map.h"
#include "Group.h"
#include "Log.h"
#include "Chat.h"
#include "InstanceScript.h"
#include "Vehicle.h"

/*
* TODO:
* 1. Dispel target regeneration
* 2. Provide unlimited http://www.wowhead.com/item=17333/aqual-quintessence
*/

namespace {

    class solocraft_player_instance_handler : public PlayerScript {
    public:
        solocraft_player_instance_handler() : PlayerScript("solocraft_player_instance_handler") {
            TC_LOG_INFO("scripts.solocraft.player.instance", "[Solocraft] solocraft_player_instance_handler Loaded");
        }

        void OnLogin(Player *player, bool /*firstLogin*/) override {
            if (sConfigMgr->GetBoolDefault("Solocraft.Enable", true))
            {
                ChatHandler(player->GetSession()).SendSysMessage("Solocraft mode activated in raids");
                _justLoggedIn = true;
                Map *map = player->GetMap();
                int difficulty = CalculateDifficulty(map, player);
                int numInGroup = GetNumInGroup(player);
                ApplyBuffs(player, map, difficulty, numInGroup);
                _justLoggedIn = false;
            }
        }

        void OnMapChanged(Player *player) override {
            if (sConfigMgr->GetBoolDefault("Solocraft.Enable", true)) {
                Map *map = player->GetMap();
                int difficulty = CalculateDifficulty(map, player);
                int numInGroup = GetNumInGroup(player);
                ApplyBuffs(player, map, difficulty, numInGroup);
            }
        }
    private:
        std::map<ObjectGuid, int> _unitDifficulty;
        bool _justLoggedIn = false;

        int CalculateDifficulty(Map *map, Player* /*player*/) {
            int difficulty = 1;
            if (map) {
                if (map->Is25ManRaid()) {
                    difficulty = 25;
                }
                else if (map->IsHeroic()) {
                    difficulty = 10;
                }
                else if (map->IsRaid()) {
                    difficulty = 40;
                }
                else if (map->IsDungeon()) {
                    difficulty = 5;
                }
            }
            return difficulty;
        }

        int GetNumInGroup(Player *player) {
            int numInGroup = 1;
            Group *group = player->GetGroup();
            if (group) {
                Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
                numInGroup = groupMembers.size();
            }
            return numInGroup;
        }

        void ApplyBuffs(Player *player, Map *map, int difficulty, int numInGroup) {
            if (!_justLoggedIn) {
                ClearBuffs(player, map);
            }
            if (difficulty > 1) {
                //InstanceMap *instanceMap = map->ToInstanceMap();
                //InstanceScript *instanceScript = instanceMap->GetInstanceScript();

                ChatHandler(player->GetSession()).PSendSysMessage("Entered %s (difficulty = %d, numInGroup = %d)",
                    map->GetMapName(), difficulty, numInGroup);

                _unitDifficulty[player->GetGUID()] = difficulty;
                for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
                    player->HandleStatFlatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float(difficulty * 100), true);
                }
                player->SetFullHealth();
                if (player->GetPowerType() == POWER_MANA) {
                    player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
                }
            }
        }

        void ClearBuffs(Player *player, Map *map) {
            std::map<ObjectGuid, int>::iterator unitDifficultyIterator = _unitDifficulty.find(player->GetGUID());
            if (unitDifficultyIterator != _unitDifficulty.end()) {
                int difficulty = unitDifficultyIterator->second;
                _unitDifficulty.erase(unitDifficultyIterator);

                ChatHandler(player->GetSession()).PSendSysMessage("Left to %s (removing difficulty = %d)",
                    map->GetMapName(), difficulty);

                for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
                    player->HandleStatFlatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float(difficulty * 100), false);
                }
            }
        }
    };

    class solocraft_pet_instance_handler : public UnitScript {
    public:
        solocraft_pet_instance_handler() : UnitScript("solocraft_pet_instance_handler") {
            TC_LOG_INFO("scripts.solocraft.pet.instance", "[Solocraft] solocraft_pet_instance_handler Loaded");
        }

        void SummonPet(Player *player, Unit *pet) override {
            if (sConfigMgr->GetBoolDefault("Solocraft.Enable", true))
            {
                Map *map = player->GetMap();
                int difficulty = CalculateDifficulty(map, player);
                ApplyBuffs(pet, difficulty);
            }
        }

        void UnsummonPet(Player* /*player*/, Unit *pet) {
            if (sConfigMgr->GetBoolDefault("Solocraft.Enable", true)) {
                ClearBuffs(pet);
            }
        }
    private:
        std::map<ObjectGuid, int> _petDifficulty;

        int CalculateDifficulty(Map *map, Player* /*player*/) {
            int difficulty = 1;
            if (map) {
                if (map->Is25ManRaid()) {
                    difficulty = 25;
                }
                else if (map->IsHeroic()) {
                    difficulty = 10;
                }
                else if (map->IsRaid()) {
                    difficulty = 40;
                }
                else if (map->IsDungeon()) {
                    difficulty = 5;
                }
            }
            return difficulty;
        }

        int GetNumInGroup(Player *player) {
            int numInGroup = 1;
            Group *group = player->GetGroup();
            if (group) {
                Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
                numInGroup = groupMembers.size();
            }
            return numInGroup;
        }

        void ApplyBuffs(Unit *pet, int difficulty) {
            if (difficulty > 1) {
                //InstanceMap *instanceMap = map->ToInstanceMap();
                //InstanceScript *instanceScript = instanceMap->GetInstanceScript();

                _petDifficulty[pet->GetGUID()] = difficulty;
                for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
                    pet->HandleStatFlatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float(difficulty * 100), true);
                }
                pet->SetFullHealth();
                if (pet->GetPowerType() == POWER_MANA) {
                    pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));
                }
            }
        }

        void ClearBuffs(Unit *pet) {
            std::map<ObjectGuid, int>::iterator petDifficultyIterator = _petDifficulty.find(pet->GetGUID());
            if (petDifficultyIterator != _petDifficulty.end()) {
                int difficulty = petDifficultyIterator->second;
                _petDifficulty.erase(petDifficultyIterator);

                for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
                    pet->HandleStatFlatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float(difficulty * 100), false);
                }
            }
        }
    };

    class solocraft_vehicle_instance_handler : public VehicleScript {
    public:
        solocraft_vehicle_instance_handler() : VehicleScript("solocraft_vehicle_instance_handler") {
            TC_LOG_INFO("scripts.solocraft.vehicle.instance", "[Solocraft] solocraft_vehicle_instance_handler Loaded");
        }

        void EnterVehicle(Vehicle *veh, Unit *passenger) override {
            if (sConfigMgr->GetBoolDefault("Solocraft.Enable", true) && veh && passenger)
            {
                Map *map = passenger->GetMap();
                int difficulty = CalculateDifficulty(map);
                ApplyBuffs(veh->GetBase(), difficulty);
            }
        }

        void ExitVehicle(Vehicle *veh, Unit* /*passenger*/) {
            if (sConfigMgr->GetBoolDefault("Solocraft.Enable", true) && veh) {
                ClearBuffs(veh->GetBase());
            }
        }
    private:
        std::map<ObjectGuid, int> _vehicleDifficulty;

        int CalculateDifficulty(Map *map) {
            int difficulty = 1;
            if (map) {
                if (map->Is25ManRaid()) {
                    difficulty = 25;
                }
                else if (map->IsHeroic()) {
                    difficulty = 10;
                }
                else if (map->IsRaid()) {
                    difficulty = 40;
                }
                else if (map->IsDungeon()) {
                    difficulty = 5;
                }
            }
            return difficulty;
        }

        int GetNumInGroup(Unit *passenger) {
            int numInGroup = 1;
            Player* player = passenger->ToPlayer();
            if (player)
            {
                Group *group = player->GetGroup();
                if (group) {
                    Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
                    numInGroup = groupMembers.size();
                }
            }
            return numInGroup;
        }

        void ApplyBuffs(Unit *veh, int difficulty) {
            if (difficulty > 1) {
                //InstanceMap *instanceMap = map->ToInstanceMap();
                //InstanceScript *instanceScript = instanceMap->GetInstanceScript();

                _vehicleDifficulty[veh->GetGUID()] = difficulty;
                for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
                    veh->HandleStatFlatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float(difficulty * 100), true);
                }
                veh->SetFullHealth();
                if (veh->GetPowerType() == POWER_MANA) {
                    veh->SetPower(POWER_MANA, veh->GetMaxPower(POWER_MANA));
                }
            }
        }

        void ClearBuffs(Unit *veh) {
            std::map<ObjectGuid, int>::iterator vehicleDifficultyIterator = _vehicleDifficulty.find(veh->GetGUID());
            if (vehicleDifficultyIterator != _vehicleDifficulty.end()) {
                int difficulty = vehicleDifficultyIterator->second;
                _vehicleDifficulty.erase(vehicleDifficultyIterator);

                for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
                    veh->HandleStatFlatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float(difficulty * 100), false);
                }
            }
        }
    };

}


void AddSC_solocraft() {
    new solocraft_player_instance_handler();
    new solocraft_pet_instance_handler();
    new solocraft_vehicle_instance_handler();
}
