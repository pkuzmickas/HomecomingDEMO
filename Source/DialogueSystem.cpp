#include "DialogueSystem.h"

unordered_map<string, Talkable::Dialogue> DialogueSystem::dialogues;
unordered_map<string, DialogueSystem::Actor> DialogueSystem::actors;
SDL_Texture* DialogueSystem::dialogueBoxTexture = NULL;
bool DialogueSystem::setuped = false;
bool DialogueSystem::open = false;
bool DialogueSystem::animating = false;
bool DialogueSystem::endReached = false;
Graphics* DialogueSystem::graphics = NULL;
Entity* DialogueSystem::dialogueBox = NULL;
Transform* DialogueSystem::dialogueBoxTransform = NULL;
Entity* DialogueSystem::speakerIMG = NULL;
Transform* DialogueSystem::speakerIMGTransform = NULL;
SDL_Texture* DialogueSystem::speakerIMGTexture = NULL;
Entity* DialogueSystem::speakerName = NULL;
Transform* DialogueSystem::speakerNameTransform = NULL;
Entity* DialogueSystem::dialogueText = NULL;
Transform* DialogueSystem::dialogueTextTransform = NULL;
Drawable* DialogueSystem::dialogueTextDrawable = NULL;

int DialogueSystem::textHeight;
int DialogueSystem::textWidth;
int DialogueSystem::curLetter = 1;
int DialogueSystem::dialogueSpeed = 50;
int DialogueSystem::lastTimeChecked = 0;
std::string DialogueSystem::curDialogueID;
int DialogueSystem::curDialogueTextIndex = 0;
int DialogueSystem::fontSize = 20;
SDL_Renderer* DialogueSystem::renderer = NULL;

void DialogueSystem::setup(Graphics* graphics, SDL_Renderer* renderer) {
	if (!setuped) {
		setuped = true;
		DialogueSystem::renderer = renderer;
		dialogueBoxTexture = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "dialogueBox.png");
		DialogueSystem::graphics = graphics; 
		DialogueSystem::dialogueBox = createDialogueBox();
	}
	else {
		cout << "Dialogue system is already setup?" << endl;
	}
}

void DialogueSystem::loadDialogues(std::string filePath) {
	ifstream theFile(filePath);
	vector<char> buffer1((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer1.push_back('\0');
	vector<char> dialogueBuffer = buffer1;
	if (dialogueBuffer.at(0) == '\0') {
		cout << "Could not load the dialogue file!" << endl;
		return;
	}

	xml_document<> doc;
	xml_node<>* root_node;
	doc.parse<0>(&dialogueBuffer[0]);
	root_node = doc.first_node("actors");
	xml_node<>* node = root_node->first_node("actor");
	while (node) {
		Actor actor;
		string id = (string)node->first_attribute("id")->value();
		actor.imgPath = (string)(node->first_attribute("img")->value());
		actor.name = (string)node->first_attribute("name")->value();
		actors[id] = actor;
		node = node->next_sibling();
	}
	node = root_node->next_sibling();
	while (node) {
		std::string dialogueID = node->first_attribute("id")->value();
		xml_node<>* textNode = node->first_node("text");
		std::vector<Talkable::Text> texts;
		while (textNode) {
			string speakerID = (string)textNode->first_attribute("speaker")->value();
			string sentence = (string)textNode->value();
			Talkable::Text text(sentence, speakerID);
			texts.push_back(text);
			textNode = textNode->next_sibling();
		}
		Talkable::Dialogue dialogue(texts, dialogueID);
		dialogues[dialogueID] = dialogue;
		node = node->next_sibling();
	}
}

void DialogueSystem::getInput() {
	std::vector<SDL_Event> events = Globals::GetFrameEvents();
	//std::cout << "EVENT SIZE IN PLAYER INPUT:" << events.size() << std::endl;
	for (auto e : events) {
		switch (e.type) {

		case SDL_MOUSEBUTTONDOWN: {
			if (animating) {
				dialogueSpeed = 0;
			}
			else {
				if (!endReached) {
					curDialogueTextIndex++;
					if (curDialogueTextIndex == dialogues[curDialogueID].dialogue.size()) {
						curDialogueTextIndex = 0;
						endReached = true;
					}
				}
				else {
					closeDialogueBox();
				}
			}


		}
						  break;
		}
	}
}

Entity * DialogueSystem::createDialogueBox()
{
	Entity* dialBox = new Entity();
	Transform* transform = new Transform(dialBox, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT / 4, CameraSystem::posX, CameraSystem::posY + Globals::SCREEN_HEIGHT - Globals::SCREEN_HEIGHT / 4);
	dialBox->addComponent(transform);
	dialogueBoxTransform = transform;
	Drawable* drawable = new Drawable(dialBox, dialogueBoxTexture, "dialogueBox", Globals::Layers::UI);
	dialBox->addComponent(drawable);
	return dialBox;
}

Entity * DialogueSystem::createSpeakerIMG(std::string speakerID) {
	Entity* speaker = new Entity();
	//60 x 39
	if (speakerIMGTexture) SDL_DestroyTexture(speakerIMGTexture);
	speakerIMGTexture = IMG_LoadTexture(renderer, actors[speakerID].imgPath.c_str());
	Transform* transform = new Transform(speaker, dialogueBoxTransform->width/6.55f, dialogueBoxTransform->height/1.61f, 0, 0);
	speakerIMGTransform = transform;
	speaker->addComponent(transform);
	Drawable* drawable = new Drawable(speaker, speakerIMGTexture, "speakerIMG", Globals::Layers::UI);
	speaker->addComponent(drawable);
	return speaker;
}

void DialogueSystem::openDialogueBox(string dialogueID) {
	open = true;
	curDialogueID = dialogueID;

	graphics->addToDraw(dialogueBox);

	speakerIMG = createSpeakerIMG(dialogues[dialogueID].dialogue[curDialogueTextIndex].speakerID);
	graphics->addToDraw(speakerIMG);
	

	std::string speakerNameText = actors[dialogues[dialogueID].dialogue[curDialogueTextIndex].speakerID].name;
	SceneDesignSystem::checkFontSizeOnText(speakerNameText, fontSize, &textWidth, &textHeight);
	speakerName = SceneDesignSystem::createText(speakerNameText, 0, 0, fontSize, { 237, 220, 188, 0xFF }, Globals::Layers::UI, renderer);
	speakerNameTransform = (Transform*)speakerName->findComponent(ComponentType::TRANSFORM);
	graphics->addToDraw(speakerName);

	std::string dialText = dialogues[dialogueID].dialogue[curDialogueTextIndex].text.substr(0, curLetter);
	dialogueText = SceneDesignSystem::createText(dialText, 0, 0, fontSize, { 126, 58, 37, 0xFF }, Globals::Layers::UI, renderer);
	dialogueTextTransform = (Transform*)dialogueText->findComponent(ComponentType::TRANSFORM);
	dialogueTextDrawable = (Drawable*)dialogueText->findComponent(ComponentType::DRAWABLE);
	graphics->addToDraw(dialogueText);

	animating = true;

}

void DialogueSystem::closeDialogueBox() {
	open = false;
	graphics->removeFromDraw(dialogueBox);
	graphics->removeFromDraw(speakerIMG);
	graphics->removeFromDraw(dialogueText);
	//TODO check if everything is cleaned up properly in cleanup() and write a switchDialogue() to switch to the next text
}

void DialogueSystem::update(float deltaTime) {
	if (open) {
		getInput();
		dialogueBoxTransform->globalPosX = CameraSystem::posX;
		dialogueBoxTransform->globalPosY = CameraSystem::posY + Globals::SCREEN_HEIGHT - Globals::SCREEN_HEIGHT / 4;
		speakerIMGTransform->globalPosX = dialogueBoxTransform->globalPosX + dialogueBoxTransform->width / 16.71f;
		speakerIMGTransform->globalPosY = dialogueBoxTransform->globalPosY + dialogueBoxTransform->height / 5.1f;
		speakerNameTransform->globalPosX = CameraSystem::posX + Globals::SCREEN_WIDTH - (dialogueBoxTransform->width / 5.8f) / 2 - textWidth / 2;
		speakerNameTransform->globalPosY = CameraSystem::posY + Globals::SCREEN_HEIGHT - dialogueBoxTransform->height / 2 - textHeight / 2;
		dialogueTextTransform->globalPosX = CameraSystem::posX + dialogueBoxTransform->width / 3.8;
		dialogueTextTransform->globalPosY = CameraSystem::posY + Globals::SCREEN_HEIGHT - dialogueBoxTransform->height + dialogueBoxTransform->height / 4.3;

		if (animating && SDL_GetTicks() > lastTimeChecked + dialogueSpeed) {
			lastTimeChecked = SDL_GetTicks();
			SDL_DestroyTexture(dialogueTextDrawable->image);
			std::string dialText = dialogues[curDialogueID].dialogue[curDialogueTextIndex].text.substr(0, curLetter);
			TTF_Font* font = TTF_OpenFont(ASSET_DIR FONT_PATH, fontSize);
			SDL_Surface* textSurface = TTF_RenderText_Solid(font, dialText.c_str(), { 126, 58, 37, 0xFF });
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
			dialogueTextDrawable->image = textTexture;
			dialogueTextTransform->width = textSurface->w;
			dialogueTextTransform->height = textSurface->h;
			SDL_FreeSurface(textSurface);
			TTF_CloseFont(font);
			curLetter++;
			if (curLetter > dialogues[curDialogueID].dialogue[curDialogueTextIndex].text.length()) {
				curLetter = 1;
				animating = false;
			}
		}


	}
}

void DialogueSystem::cleanup() {
	if (setuped) {
		dialogues.clear();
		actors.clear();
		SDL_DestroyTexture(dialogueBoxTexture);
		
		if (dialogueBox) delete dialogueBox;
		if (speakerIMGTexture) {
			SDL_DestroyTexture(speakerIMGTexture);
			delete speakerIMG;
		}
		if (speakerName) {
			SceneDesignSystem::cleanupText(speakerName);
		}
		if (dialogueText) {
			SceneDesignSystem::cleanupText(dialogueText);
		}
		setuped = false;
	}
}
