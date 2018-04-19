#include "DialogueSystem.h"

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
	xml_node<>* layer = root_node->first_node("actor");
	while (layer) {
		Actor actor;
		string id = (string)layer->first_attribute("id")->value();
		actor.imgPath = (string)(layer->first_attribute("img")->value());
		actor.name = (string)layer->first_attribute("name")->value();
		speakers[speaker->id] = speaker;
		layer = layer->next_sibling();
	}
	layer = root_node->next_sibling();
	while (layer) {
		Dialogue* dialogue = new Dialogue();
		dialogue->id = (string)layer->first_attribute("id")->value();
		dialogue->speaker = speakers[(string)layer->first_attribute("speaker")->value()];
		xml_node<>* textNode = layer->first_node("text");
		dialogue->text = (string)textNode->value();
		dialogues[dialogue->id] = dialogue;
		layer = layer->next_sibling();
	}
}
