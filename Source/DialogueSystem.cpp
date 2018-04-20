#include "DialogueSystem.h"

unordered_map<string, Talkable::Dialogue> DialogueSystem::dialogues;
unordered_map<string, DialogueSystem::Actor> DialogueSystem::actors;
SDL_Texture* DialogueSystem::dialogueBoxIMG = NULL;
bool DialogueSystem::setuped = false;
bool DialogueSystem::open = false;

void DialogueSystem::setup(SDL_Texture * dialogueBox) {
	if (!setuped) {
		setuped = true;
		dialogueBoxIMG = dialogueBox;
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
		xml_node<>* textNode = root_node->first_node("text");
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
	Drawable* drawable = new Drawable(dialBox, dialogueBoxIMG, "dialogueBox", Globals::Layers::UI);
	dialBox->addComponent(drawable);
	return dialBox;
}

void DialogueSystem::openDialogueBox() {
	open = true;
}

void DialogueSystem::closeDialogueBox() {
	open = false;
}

void DialogueSystem::update(float deltaTime) {
}

void DialogueSystem::cleanup() {
	if (setuped) {
		dialogues.clear();
		actors.clear();
		SDL_DestroyTexture(dialogueBoxIMG);
		setuped = false;
	}
}
