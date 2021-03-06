#pragma once

#include "../engine/core.hpp"

#include "util/button.hpp"
#include "unitData.hpp"

#include <vector>
#include <string>

#define DEFAULT_PLAYER_FILE		"res/data/save.json"
#define DEFAULT_MASTER_FILE		"res/data/levels/master.json"
#define DEFAULT_ATTACK_FILE		"res/data/attacks.json"

#define DEFAULT_MAX_EXP			500.f

class SkillTreeLinkButton : public ButtonData {
public:
	SkillTreeLinkButton(Vec2<int> position = Vec2<int>(0, 0), int width = 160, int height = 80);
	void render();
};

class Customization : public State {

public:

	Customization(const std::string& file = DEFAULT_PLAYER_FILE);
	~Customization();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

private:

	std::vector<UnitData> units;
	// the available moves to each unit
	std::vector<std::vector<std::string>> attacks;

	void displayUnitData(const UnitData& data);
	void renderUnit(int x, int y, UnitData unit);
	void renderEmpty(int x, int y);
	void generateDefaultUnitData();

	// Buttons used to bring to next state
	SkillTreeLinkButton button1;
	SkillTreeLinkButton button2;
	SkillTreeLinkButton button3;
	SkillTreeLinkButton button4;
	ButtonData continueButton;

	void initSprites();
	// Sprites used to render the customization state
	Sprite base;
	Sprite empty;

	// Helper functions
	void switchToCombatState();
	void initAvailableAttacks();

};