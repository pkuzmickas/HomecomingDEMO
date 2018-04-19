#include "DialogueSystem.h"

unordered_map<string, vector<Talkable::Dialogue>> DialogueSystem::dialogues;
unordered_map<string, DialogueSystem::Actor> DialogueSystem::actors;

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
		string speakerID = (string)node->first_attribute("speaker")->value();
		string sentence = (string)node->value();
		Talkable::Dialogue dialogue(sentence);
		dialogues[speakerID].push_back(dialogue);
		node = node->next_sibling();
	}
}

Entity * DialogueSystem::createDialogueBox(SDL_Texture* dialogueIMG)
{
	Entity* dialBox = new Entity();
	Transform* transform = new Transform(dialBox, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT / 4, CameraSystem::posX, CameraSystem::posY + Globals::SCREEN_HEIGHT - Globals::SCREEN_HEIGHT / 4);
	dialBox->addComponent(transform);
	Drawable* drawable = new Drawable(dialBox, dialogueIMG, "dialogueBox", Globals::Layers::UI);
	dialBox->addComponent(drawable);
	return dialBox;
}

Entity * DialogueSystem::closeDialogueBox()
{
	return nullptr;
}

void DialogueSystem::update(float deltaTime) {
}
