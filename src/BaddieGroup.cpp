#include "BaddieGroup.hpp"

BaddieGroup::BaddieGroup() {
	textureArray[0].loadFromFile("res/resource/invader3.png");
	textureArray[1].loadFromFile("res/resource/invader2.png");
	textureArray[2].loadFromFile("res/resource/invader1.png");

	for (int x = 0; x < COLUMNS; x++) {
		for (int y = 0; y < ROWS; y++) {
			baddies[x][y] = new Baddie(textureArray[y], x * 12, 4 + (y * 14));
		}
	}
}

bool BaddieGroup::testHit(Bullet &pew) {
	for (int x = 0; x < COLUMNS; x++) {
		for (int y = 0; y < ROWS; y++) {
			if (baddies[x][y]){
				if (baddies[x][y]->collision().intersects(pew.collision())) {
					delete baddies[x][y];
					baddies[x][y] = nullptr;
					didDieThisFrame = true;
					return true;
				}	
			}
		}
	}
	return false;
}

bool BaddieGroup::baddiesWin() {
	for (int y = ROWS - 1; y >= 0; y--) {	
		for (int x = COLUMNS - 1; x >= 0; x--) {
			if (baddies[x][y]){
				return (baddies[x][y]->getY() >= 120);				
			}
		}	
	}
	return false;
}

void BaddieGroup::update() {
	bool allMoveDown = false;
	for (int x = 0; x < COLUMNS; x++) {
		for (int y = 0; y < ROWS; y++) {
			if (baddies[x][y]){
				if (allMoveRight) {
					baddies[x][y]->moveRight();
					if (baddies[x][y]->getX() >= 120) {
						allMoveDown = true;
					}
				}
				else if(!allMoveRight) {
					baddies[x][y]->moveLeft();
					if (baddies[x][y]->getX() <= 0) {
						allMoveDown = true;
					}
				}
			}	
		}
	}
	if (allMoveRight && allMoveDown){
		for (int x = 0; x < COLUMNS; x++) {
			for (int y = 0; y < ROWS; y++) {
				if (baddies[x][y]){
					baddies[x][y]->moveDown();
				}
			}			
		}
		allMoveDown = false;
		allMoveRight = false;
	}
	if (!allMoveRight && allMoveDown){
		for (int x = 0; x < COLUMNS; x++) {
			for (int y = 0; y < ROWS; y++) {
				if (baddies[x][y]){
					baddies[x][y]->moveDown();
				}
			}			
		}
		allMoveDown = false;
		allMoveRight = true;
	}
	if (didDieThisFrame) {
		for (int x = 0; x < COLUMNS; x++) {
			for (int y = 0; y < ROWS; y++) {
				if (baddies[x][y]){
					baddies[x][y]->speed +=.02;
					cout << currentBaddies << endl;
				}
			}
		}
		didDieThisFrame = false;
		currentBaddies--;			
	}

}


void BaddieGroup::draw(RenderWindow& window) {
	for (int x = 0; x < COLUMNS; x++) {
		for (int y = 0; y < ROWS; y++) {
			if (baddies[x][y])
				baddies[x][y]->draw(window);
		}
	}
}