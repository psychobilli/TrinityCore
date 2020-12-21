DELETE FROM `quest_template_addon` WHERE `ID` IN (12229, 12231);

INSERT INTO `quest_template_addon`
(`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`)
VALUES(12229,0,0,0,0,12242,0,0,0,0,0,0,0,0,0,0,0,0)
	, (12231,0,0,0,0,12241,0,0,0,0,0,0,0,0,0,0,0,0);

UPDATE `quest_template_addon` SET `NextQuestID` = 12231 WHERE `ID` = 12207;
UPDATE `quest_template_addon` SET `NextQuestID` = 12229 WHERE `ID` = 12213;

UPDATE `quest_template_addon` SET `NextQuestID` = 12255 WHERE `ID` = 12222;
UPDATE `quest_template_addon` SET `NextQuestID` = 12255 WHERE `ID` = 12223;

UPDATE `smart_scripts` SET `target_type` = 27 WHERE `entryorguid` IN (27718,27715,27717,27716,27727) AND `source_type` = 0 AND `event_type` = 6 AND `action_type` = 15;
