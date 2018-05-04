#include "AISoldier.h"

AISoldier::AISoldier(Entity * owner) : AIComponent(owner) {

}

void AISoldier::update(float deltaTime) {
	AIComponent::update(deltaTime);
	if (state == ATTACKING) {
		if (subState == NONE) {
			subState = FINDING;
			walkTo(targetTransform->globalPosX, targetTransform->globalPosY, 100);
		}
		if (subState == FINDING) {
			if (curPathIndex == path.size()-3) {
				calculatePath(targetTransform->globalPosX, targetTransform->globalPosY);
			}
			else if(path.size()==2) {
				stopWalking();
				cout << "SLASH ATTACK!" << endl; //TODO instead of stopwalking have recalculate(), check the direction bug
			}
		}
	}
	
}



