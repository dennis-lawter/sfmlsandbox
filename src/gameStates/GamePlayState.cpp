#include "GamePlayState.hpp"

GamePlayState::GamePlayState() {
	startMusic.setBuffer(resources::soundFile["randommusic"]);
	startMusic.play();
	score::scoreBonus = 10;

	pauseTint.setFillColor(Color(0x000000D0));
	pauseTint.setSize(Vector2f(defines::WIDTH, defines::HEIGHT));
	pauseTint.setPosition(0, 0);

	pause.setString("PAUSE");
	pause.setFont(resources::font);
	pause.setCharacterSize(80);
	pause.setScale(.12, .12);
	pause.setPosition(40, 55);

	// demonstration of a blood splatter
	// for (int i = 0; i < 15; i++) {
	// 	Color blood (0xff0000ff);
	// 	blood.r = util::rangedRand(128, 255);
	// 	ParticleAttributeList p = {
	// 		// {64.f, 64.f},
	// 		{util::rangedRand(-25, 25), util::rangedRand(-25, 25)},
	// 		// {util::rangedRand(-10, 10)/10.f, util::rangedRand(-10, 10)/10.f},
	// 		{0.f, 0.f},
	// 		// {util::rangedRand(-10, 10)/100.f, util::rangedRand(-10, 10)/100.f},
	// 		{0.f, 0.f},
	// 		120,
	// 		blood,
	// 		Color(0x00000000),
	// 		12.f,
	// 		0.f
	// 	};
	// 	particles.createParticle(p);
	// }
}

void GamePlayState::startRound() {
	if (roundStartTimer < ROUND_START_MAX) {
		if (roundStartTimer < BLINK_MAX && (roundStartTimer / BLINK_SPEED) % 2 == 0) {
			roundTitle.str("");
			roundTitle << "ROUND   " << score::roundNumber;
		} else if (roundStartTimer < BLINK_MAX) {
			roundTitle.str("");
		}

		roundStartTimer++;

		killemAll.animateIntro(roundStartTimer);
		defender.animateIntro(roundStartTimer);
		saveMe.animateIntro(roundStartTimer);

		return;
	} else {
		roundTitle.str("");
		roundStart = false;
	}
	return;
}

void GamePlayState::endRound() {
	defender.animateOutro(endRoundBuffer);
	endRoundBuffer++;
}

void GamePlayState::updateComponents() {
	// update state components
	defender.update();
	killemAll.update();
	particles.update();
	if (isUfoMoving && ufo) {
		ufo->update();
	}
	if (powerup) {
		powerup->update();
	}
}

void GamePlayState::detectCollisions() {
	// do collision tests

	// defender's bullet collision tests
	defender.testBulletCollisions(killemAll, saveMe, particles);

	// defender touches baddie bullet
	if (defender.testManyForCollisionWithBaddieBullet(killemAll.bulletVector) && !defender.isInvuln) {
		score::currentLives--;
		defender.isInvuln = true;
	}

	// barrier touches baddie bullet
	saveMe.testManyForCollisionWithBaddieBullet(killemAll.bulletVector, true, true);
	// barrier touches baddie
	killemAll.testManyForCollisionWithBarrier(saveMe.barrierVector, false, true);

	// powerup touches defender 
	if (powerup && defender.testCollision(*powerup) && (randomPowerup == defines::PowerUp::Aggressive || randomPowerup == defines::PowerUp::Passive)) { // add tests to make sure powerup is agressive, passive
		for (int x = 0; x < killemAll.COLUMNS; x++) {
			for (int y = 0; y < killemAll.ROWS; y++) {
				if (killemAll.baddies[x][y] && randomPowerup == defines::PowerUp::Aggressive) {
					powerup->agressive(*killemAll.baddies[x][y]);
				} else if (killemAll.baddies[x][y] && randomPowerup == defines::PowerUp::Passive) {
					powerup->passive(*killemAll.baddies[x][y]);
				}
			}
		}
		delete powerup;
		powerup = nullptr;
	} else if (powerup && defender.testCollision(*powerup)) {
		powerup->grantPowerUp();
		delete powerup;
		powerup = nullptr;
	}
}

void GamePlayState::removeOffscreenAssets() {
	// powerup off screen
	if (powerup) {
		if (powerup->isOffScreen()) {
			delete powerup;
			powerup = nullptr;
		}
	}

	//ufo despawns off screen
	if (ufo && ufo->isOffScreen()) {
		delete ufo;
		ufo = nullptr;
		isUfoMoving = false;
	}
}

void GamePlayState::calculateUfo() {
	//start ufo timer when baddies advance
	if (killemAll.baddiesTimesAdvanced > 0) {
		if (setUfoRandom > ufoBuffer && setUfoTimer) {
			ufoBuffer++;
		} else if (ufoBuffer >= setUfoRandom && setUfoTimer) {
			ufo = new Ufo();
			setUfoTimer = false;
			isUfoMoving = true;
			setUfoRandom = 0;
			ufoBuffer = 0;
		} else if (!setUfoTimer && !isUfoMoving) {
			setUfoTimer = true;
			setUfoRandom = rand() % (UFO_TIMER_MAX - UFO_TIMER_MIN) + UFO_TIMER_MIN;
		}
	}

	//ufo fires powerup
	if (ufo && ufo->hasFired && !powerup && !didUfoFire) {
		randomPowerup = static_cast<defines::PowerUp>(rand() % defines::PowerUp::COUNT);
		powerup = new Powerup(randomPowerup, ufo->getX(), defender);
		didUfoFire = true;
	}

	//ufo & powerup reset
	if (!powerup && !ufo) {
		didUfoFire = false;
		randomPowerup = defines::PowerUp::COUNT;
	}
}

void GamePlayState::calculateStateStatus() {
	// test if state has been completed
	if (killemAll.currentBaddies <= 0) {
		didWin = true;
		if (endRoundBuffer >= END_ROUND_TIMER) {
			isEnding = true;
		}
	} else if (killemAll.isBaddiesWin() || score::currentLives < 0) {
		didWin = false;
		isEnding = true;
	}
}

void GamePlayState::processInput(Event& event) {
	if (roundStart) {
		return;
	}
	if (isPause && event.key.code != Keyboard::Escape) {
		return;
	}
	switch (event.type) {
	case Event::KeyPressed:
		switch (event.key.code) {
		// God Button
		// case Keyboard::BackSlash:
		// 	defender.isPunch = true;
		// 	break;
		case Keyboard::A:
		case Keyboard::Left: //Move Left
			defender.playerIsMovingLeft = true;
			break;
		case Keyboard::D:
		case Keyboard::Right: //Move Right
			defender.playerIsMovingRight = true;
			break;
		case Keyboard::W: //Player fires
		case Keyboard::Up:
		case Keyboard::Space:
			defender.fire();
			break;
		case Keyboard::Escape:
			isPause = !isPause;
			defender.playerIsMovingLeft = false;
			defender.playerIsMovingRight = false;
		default:
			break;
		}
		break;

		/* This case allows for smooth player movement left and right
		*/
	case Event::KeyReleased:
		switch (event.key.code) {
		case Keyboard::Key::A:
		case Keyboard::Key::Left:
			defender.playerIsMovingLeft = false;
			break;
		case Keyboard::Key::D:
		case Keyboard::Key::Right:
			defender.playerIsMovingRight = false;
			break;
		default:
			break;
		}
	default:
		break;
	}

}

void GamePlayState::update(RenderWindow& window) {
	if (isPause) {
		return;
	}
	if (roundStart) {
		startRound();
		return;
	}
	if (didWin) {
		endRound();
		calculateStateStatus();
		return;
	}

	updateComponents();
	detectCollisions();
	removeOffscreenAssets();
	calculateUfo();
	calculateStateStatus();
}

void GamePlayState::draw(RenderWindow& window) {
	hud.draw(window);
	defender.draw(window);
	killemAll.draw(window);
	saveMe.draw(window);
	particles.draw(window);
	Text drawTitle1 = Text(roundTitle.str(), resources::font, 80);
	drawTitle1.setScale(.08, .08);
	drawTitle1.setPosition(30, 50);
	window.draw(drawTitle1);
	if (ufo) {
		ufo->draw(window);
	}
	if (powerup) {
		powerup->draw(window);
	}
	if (isPause) {
		window.draw(pauseTint);
		window.draw(pause);
	}
}

GamePlayState::~GamePlayState() {}
