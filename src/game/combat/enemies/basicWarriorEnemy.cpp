#include "basicWarriorEnemy.hpp"

#include "../../combat.hpp"

#include "../../util/attackloader.hpp"
#include "../player.hpp"


WarriorEnemy::WarriorEnemy() : 
	// Init class vars
	Enemy(UnitType::ENEMY, "res/assets/enemies/WyvernFighter_Sprite.png"),
	// Attack init
	hit(Attacks::get("HIT", this)),
	block(Attacks::get("BLOCK", this))
{
	sprite.setSize(sprite_width, sprite_height);
}

WarriorEnemy::~WarriorEnemy() {
}

//calculates euclidian distance between 2 positions
int distance(Vec2<int> p1, Vec2<int> p2){
	return sqrt(pow(p1[0]-p2[0],2)-pow(p1[1]-p2[1],2));
}	

void WarriorEnemy::handleMovement() {

	/*
		- The warrior enemy should simply move towards the player every turn

		- If the warrior enemy is already beside a player, then we should go
			directly to the handle attack state
	*/


	int moveSpeed = 2;
	int min_distance = INT_MAX;
	int max_grid = 4;

	//target the closest player
	Player * target_player = NULL;
	std::vector<Player*> players = combat->getPlayers();
	for (Player *& p : players){	
		if(p->getType() == UnitType::PLAYER){
			if(distance(position,p->position)<min_distance){
				target_player = p;
				min_distance=distance(position,p->position);
			}
		}
	}

	Vec2<int> target_position;

	//set target_position one of these: left,right,top,bottom of target_player
	//according to which one is closer
	min_distance=INT_MAX;
	Vec2<int> temp_target =target_position;

	temp_target = {target_player->position[0]-1,target_player->position[1]};
	if(combat->isPosEmpty(temp_target) && distance(temp_target,position)<min_distance){
		min_distance=distance(temp_target,position);
		target_position=temp_target;
	}
	temp_target = {target_player->position[0]+1,target_player->position[1]};
	if(combat->isPosEmpty(temp_target) &&distance(temp_target,position)<min_distance){
		min_distance=distance(temp_target,position);
		target_position=temp_target;
	}
	temp_target = {target_player->position[0],target_player->position[1]-1};
	if(combat->isPosEmpty(temp_target) &&distance(temp_target,position)<min_distance){
		min_distance=distance(temp_target,position);
		target_position=temp_target;
	}
	temp_target = {target_player->position[0],target_player->position[1]+1};
	if(combat->isPosEmpty(temp_target) &&distance(temp_target,position)<min_distance){
		min_distance=distance(temp_target,position);
		target_position=temp_target;
	}

	//set the actual position to one in range in case the target_position is out of range
	Vec2<int> position_within_range=target_position;
	if(getPath(*combat, target_position).size()>(moveSpeed+1)){
		position_within_range = {getPath(*combat, target_position)[moveSpeed][0],getPath(*combat, target_position)[moveSpeed][1]};
		printf("%d,%d\n",position_within_range[0],position_within_range[1]);
	}	

	if (!move(*combat,position_within_range)) {
		printf("failed move\n");
        // Directly handle the attacks if no movement could be done
        handleAttack();
    }
}

void WarriorEnemy::handleAttack() {
	// TODO: Implement BLOCK

	/*
		- The warrior enemy has 2 basic attacks: HIT and BLOCK

		- The HIT attack is just a basic melee attack that hits players
		
		- The BLOCK attack should apply a passive effect on the enemy that
			halves incoming damage
		- Passive effects are not implemented in the game yet, so this can
			not be implemneted right now

		- The choosing of which attack to use can be random OR based on player
			position or something. Creative freedom is up to you!
	*/
	// If there is a player adjacent to the enemy, attack the player
    if (combat->getUnitAt(position - Vec2<int>(1, 0))) {
        hit.attack(position - Vec2<int>(1, 0), *combat);
        state = UnitState::ATTACK;
        startCounter();
        return;
    }
    if (combat->getUnitAt(position - Vec2<int>(0, 1))) {
        hit.attack(position - Vec2<int>(0, 1), *combat);
        state = UnitState::ATTACK;
        startCounter();
        return;
    }
    if (combat->getUnitAt(position - Vec2<int>(-1, 0))) {
        hit.attack(position - Vec2<int>(-1, 0), *combat);
        state = UnitState::ATTACK;
        startCounter();
        return;
    }
    if (combat->getUnitAt(position - Vec2<int>(0, -1))) {
        hit.attack(position - Vec2<int>(0, -1), *combat);
        state = UnitState::ATTACK;
        startCounter();
        return;
    }
    // If no attacks could be done, set the unit to be at done state
	state = UnitState::DONE;
}
