#include "AISoldier.h"

AISoldier::AISoldier(Entity * owner) : AIComponent(owner) {

}

void AISoldier::update(float deltaTime) {
	AIComponent::update(deltaTime);
	if (state == ATTACKING) {
		int chaseSpeed = 200;
		if (subState == NONE) {
			subState = FINDING;
			walkTo(targetTransform->globalPosX, targetTransform->globalPosY, chaseSpeed);
		}
		if (subState == FINDING) {
			if (curPathIndex == path.size()-3) {
				calculatePath(targetTransform->globalPosX, targetTransform->globalPosY);
				if (!walking) {
					walkTo(targetTransform->globalPosX, targetTransform->globalPosY, chaseSpeed);
				}
			}
			else if(path.size()==3) {
				stopWalking();
				subState = SLASHING;
			}
			if (isKnocked()) {
				calculatePath(targetTransform->globalPosX, targetTransform->globalPosY);
			}
		}
		if (subState == SLASHING) {
			calculatePath(targetTransform->globalPosX, targetTransform->globalPosY);
			if (path.size() > 3) {
				subState = NONE;
			}
			cout << "SLASH ATTACK!" << endl; //TODO instead of stopwalking have recalculate(), check the direction bug
		}
	}
	
}



