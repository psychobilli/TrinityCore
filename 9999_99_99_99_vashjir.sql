-- Correct Call of Duty (25924, 14482)
set @EasternKingdoms := 0;
set @HrdQstGvrEntry := 41621;
set @HrdCoDQuestId := 25924;
set @HrdErunakCreatureId := 41618;
set @HrdSeeErunakSpell := 77790;
set @HrdPosX := -4615.24;
set @HrdPosY := 3982.79;
set @HrdPosZ := -70.554;
set @HrdPosO := 5.13391;

update `creature_template` set `AIName` = 'SmartAI' where `entry` = @HrdQstGvrEntry;

delete from `smart_scripts` where `entryorguid` = @HrdQstGvrEntry and `source_type` = 0;
insert into `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@HrdQstGvrEntry, 0, 0, 0, 19, 0, 100, 512, @HrdCoDQuestId, 0, 0, 0, '', 28, @HrdSeeErunakSpell, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Commander Thorak - Accepted Quest - Remove Aura'),
(@HrdQstGvrEntry, 0, 1, 0, 19, 0, 100, 512, @HrdCoDQuestId, 0, 0, 0, '', 75, @HrdSeeErunakSpell, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Commander Thorak - Accepted Quest - Add Aura'),
(@HrdQstGvrEntry, 0, 2, 0, 19, 0, 100, 512, @HrdCoDQuestId, 0, 0, 0, '', 62, @EasternKingdoms, 0, 0, 0, 0, 0, 7, 0, 0, 0, @HrdPosX, @HrdPosY, @HrdPosZ, @HrdPosO, 'Commander Thorak - Accepted Quest - Teleport');

update `quest_objectives` set `type` = 5, `ObjectID` = @HrdSeeErunakSpell where `QuestID` = @HrdCoDQuestId;

set @AlyQstGvrEntry := 36799;
set @AlyCoDQuestId := 14482;
set @AlyErunakCreatureId := 36915;
set @AlySeeErunakSpell := 75974;
set @AlyPosX := -4455.4;
set @AlyPosY := 3806.15;
set @AlyPosZ := -82.5101;
set @AlyPosO := .518756;

update `creature_template` set `AIName` = 'SmartAI' where `entry` = @AlyQstGvrEntry;

delete from `smart_scripts` where `entryorguid` = @AlyQstGvrEntry and `source_type` = 0;
insert into `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@AlyQstGvrEntry, 0, 0, 0, 19, 0, 100, 512, @AlyCoDQuestId, 0, 0, 0, '', 28, @AlySeeErunakSpell, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Recruiter Burns - Accepted Quest - Remove Aura'),
(@AlyQstGvrEntry, 0, 1, 0, 19, 0, 100, 512, @AlyCoDQuestId, 0, 0, 0, '', 75, @AlySeeErunakSpell, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Recruiter Burns - Accepted Quest - Add Aura'),
(@AlyQstGvrEntry, 0, 2, 0, 19, 0, 100, 512, @AlyCoDQuestId, 0, 0, 0, '', 62, @EasternKingdoms, 0, 0, 0, 0, 0, 7, 0, 0, 0, @AlyPosX, @AlyPosY, @AlyPosZ, @AlyPosO, 'Recruiter Burns - Accepted Quest - Teleport');

update `quest_objectives` set `type` = 5, `ObjectID` = @AlySeeErunakSpell where `QuestID` = @AlyCoDQuestId;

-- # ID, QuestID, Type, Order, StorageIndex, ObjectID, Amount, Flags, Flags2, ProgressBarWeight, Description, VerifiedBuild
-- '266167', '14482', '0', '0', '0', '36915', '1', '0', '0', '0', 'Ride the mercenary ship to Vashj\'ir', '25549'
-- '266797', '25924', '0', '0', '0', '36901', '1', '0', '0', '0', 'Ride the mercenary ship to Vashj\'ir', '25549'
