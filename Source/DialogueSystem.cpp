#include "DialogueSystem.h"

unordered_map<string, Talkable::Dialogue> DialogueSystem::dialogues;
unordered_map<string, DialogueSystem::Actor> DialogueSystem::actors;
SDL_Texture* DialogueSystem::dialogueBoxTexture = NULL;
bool DialogueSystem::setuped = false;
bool DialogueSystem::open = false;
Graphics* DialogueSystem::graphics = NULL;
Entity* DialogueSystem::dialogueBox = NULL;
Transform* DialogueSystem::dialogueBoxTransform = NULL;
Entity* DialogueSystem::speakerIMG = NULL;
Transform* DialogueSystem::speakerIMGTransform = NULL;
SDL_Texture* DialogueSystem::speakerIMGTexture = NULL;
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
	Transform* transform = new Transform(speaker, dialogueBoxTransform->width/6.55f, dialogueBoxTransform->height/1.61f, dialogueBoxTransform->globalPosX + dialogueBoxTransform->width/16.71f, dialogueBoxTransform->globalPosY + dialogueBoxTransform->height / 5.1f);
	speakerIMGTransform = transform;
	speaker->addComponent(transform);
	Drawable* drawable = new Drawable(speaker, speakerIMGTexture, "speakerIMG", Globals::Layers::UI);
	speaker->addComponent(drawable);
	return speaker;
}

void DialogueSystem::openDialogueBox(string dialogueID) {
	open = true;
	graphics->addToDraw(dialogueBox);
	speakerIMG = createSpeakerIMG(dialogues[dialogueID].dialogue[0].speakerID);
	graphics->addToDraw(speakerIMG);
}

void DialogueSystem::closeDialogueBox() {
	open = false;
	graphics->removeFromDraw(dialogueBox);
	graphics->removeFromDraw(speakerIMG);
}

void DialogueSystem::update(float deltaTime) {
	if (open) {
		dialogueBoxTransform->globalPosX = CameraSystem::posX;
		dialogueBoxTransform->globalPosY = CameraSystem::posY + Globals::SCREEN_HEIGHT - Globals::SCREEN_HEIGHT / 4;
		speakerIMGTransform->globalPosX = dialogueBoxTransform->globalPosX + dialogueBoxTransform->width / 16.71f;
		speakerIMGTransform->globalPosY = dialogueBoxTransform->globalPosY + dialogueBoxTransform->height / 5.1f;
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
		setuped = false;
	}
}
