#ifndef GAMEPLAYSTATE_HPP_
#define GAMEPLAYSTATE_HPP_
#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include "../gameObj/Player.hpp"
#include "../gameObj/PlayerBullet.hpp"
#include "../gameObj/Barrier.hpp"
#include "../BaddieGroup.hpp"
#include "../Hud.hpp"
#include "../BarrierGroup.hpp"

using namespace sf;

class GamePlayState : public GameState {
protected:
	Player* defender = nullptr;
	PlayerBullet* pew = nullptr;
	BaddieGroup* killemAll = nullptr;
	Hud* hud = nullptr;
	BarrierGroup* saveMe = nullptr;
public:
	bool didWin = false;

	GamePlayState();

	void processInput(Event& event);
	void update(RenderWindow& window);
	void draw(RenderWindow& window);

	virtual ~GamePlayState();
	// 	delete defender;
	// delete killemAll;
	// if (pew)
	// 	delete pew;
};

#endif