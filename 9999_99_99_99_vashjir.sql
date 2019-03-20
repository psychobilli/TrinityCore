-- Correct Eye of the Storm (28805, 28826)
set @WaterOfFarseeingKC := 50054;
set @HrdWaterOfFarseeingEntry := 207414;
set @AlyWaterOfFarseeingEntry := 207416;

update `gameobject_template` set `AIName` = 'SmartGameObjectAI' where `entry` in (@HrdWaterOfFarseeingEntry,@AlyWaterOfFarseeingEntry);

delete from `smart_scripts` where `entryorguid` in (@HrdWaterOfFarseeingEntry, @AlyWaterOfFarseeingEntry) and `source_type` = 1;
delete from `smart_scripts` where `entryorguid` = @WaterOfFarseeingKC and `source_type` = 0;
insert into `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@HrdWaterOfFarseeingEntry, 1, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, '', 45, 1, 1, 0, 0, 0, 0, 10, 0, @WaterOfFarseeingKC, 0, 0, 0, 0, 0, 'Waters of Farseeing - Gameobject Gossip Hello - Set Data 1 1'),
(@HrdWaterOfFarseeingEntry, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, '', 33, @WaterOfFarseeingKC, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 'Waters of Farseeing - Link Gameobject Gossip Hello - Waters of Farseeing Kill Credit'),
(@AlyWaterOfFarseeingEntry, 1, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, '', 45, 1, 1, 0, 0, 0, 0, 10, 0, @WaterOfFarseeingKC, 0, 0, 0, 0, 0, 'Waters of Farseeing - Gameobject Gossip Hello - Set Data 1 1'),
(@AlyWaterOfFarseeingEntry, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, '', 33, @WaterOfFarseeingKC, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 'Waters of Farseeing - Link Gameobject Gossip Hello - Waters of Farseeing Kill Credit'),
(@WaterOfFarseeingKC, 0, 0, 1, 38, 0, 100, 512, 1, 1, 0, 0, '', 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waters of Farseeing Kill Credit - On Data Set 1 1 - Set Data 1 0');

-- Correct Alliance To The Depths availability
set @AlyEyeOTStorm := 28826;
set @AlyToTheDepths := 28827;
update `quest_template_addon` set `PrevQuestId` = @AlyEyeOTStorm where `ID` = @AlyToTheDepths;

-- Correct Call of Duty (25924, 14482)
set @EasternKingdoms := 0;
set @NtrlBtrLtTDead := 25477;

set @HrdQstGvrEntry := 41621;
set @HrdCoDQuestId := 25924;
set @HrdErunakCreatureId := 41618;
set @HrdSeeErunakSpell := 77790;
set @HrdQuestHubArea := 5056;
set @HrdPosX := -4615.24;
set @HrdPosY := 3982.79;
set @HrdPosZ := -70.554;
set @HrdPosO := 5.13391;

update `creature_template` set `AIName` = 'SmartAI' where `entry` = @HrdQstGvrEntry;

delete from `smart_scripts` where `entryorguid` = @HrdQstGvrEntry and `source_type` = 0;
insert into `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@HrdQstGvrEntry, 0, 0, 0, 19, 0, 100, 512, @HrdCoDQuestId, 0, 0, 0, '', 62, @EasternKingdoms, 0, 0, 0, 0, 0, 7, 0, 0, 0, @HrdPosX, @HrdPosY, @HrdPosZ, @HrdPosO, 'Commander Thorak - Accepted Quest - Teleport');

delete from `spell_area` where `spell` = @HrdSeeErunakSpell;
insert into `spell_area`
(`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) values
(@HrdSeeErunakSpell, @HrdQuestHubArea, @HrdCoDQuestId, @NtrlBtrLtTDead, 0, 0, 2, 1, 74, 11);

update `quest_objectives` set `type` = 5, `ObjectID` = @HrdSeeErunakSpell, `Amount` = 0 where `QuestID` = @HrdCoDQuestId;

set @AlyQstGvrEntry := 36799;
set @AlyCoDQuestId := 14482;
set @AlyErunakCreatureId := 36915;
set @AlySeeErunakSpell := 75974;
set @AlyQuestHubArea := 5012;
set @AlyPosX := -4455.4;
set @AlyPosY := 3806.15;
set @AlyPosZ := -82.5101;
set @AlyPosO := .518756;

update `creature_template` set `AIName` = 'SmartAI' where `entry` = @AlyQstGvrEntry;

delete from `smart_scripts` where `entryorguid` = @AlyQstGvrEntry and `source_type` = 0;
insert into `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@AlyQstGvrEntry, 0, 1, 0, 19, 0, 100, 512, @AlyCoDQuestId, 0, 0, 0, '', 62, @EasternKingdoms, 0, 0, 0, 0, 0, 7, 0, 0, 0, @AlyPosX, @AlyPosY, @AlyPosZ, @AlyPosO, 'Recruiter Burns - Accepted Quest - Teleport');

delete from `spell_area` where `spell` = @AlySeeErunakSpell;
insert into `spell_area`
(`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) values
(@AlySeeErunakSpell, @AlyQuestHubArea, @AlyCoDQuestId, @NtrlBtrLtTDead, 0, 0, 2, 1, 74, 11);

update `quest_objectives` set `type` = 5, `ObjectID` = @AlySeeErunakSpell, `Amount` = 0  where `QuestID` = @AlyCoDQuestId;

-- # ID, QuestID, Type, Order, StorageIndex, ObjectID, Amount, Flags, Flags2, ProgressBarWeight, Description, VerifiedBuild
-- '266167', '14482', '0', '0', '0', '36915', '1', '0', '0', '0', 'Ride the mercenary ship to Vashj\'ir', '25549'
-- '266797', '25924', '0', '0', '0', '36901', '1', '0', '0', '0', 'Ride the mercenary ship to Vashj\'ir', '25549'
