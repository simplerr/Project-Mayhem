#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"
#include "Graphics.h"
#include "ObjectHandler.h"
#include "Inventory.h"

ObjectHandler::ObjectHandler()
{
	// Load all different items
	TiXmlDocument doc("Data\\objects.xml");
	doc.LoadFile();

	// Get the root element
	TiXmlElement* root = doc.FirstChildElement();

	// Load items from the file
	for(TiXmlElement* item = root->FirstChildElement("Object"); item != NULL; item = item->NextSiblingElement("Object"))
	{
		ObjectData objectData;
		objectData.name = item->Attribute("name") == NULL ? "#NOVALUE" : item->Attribute("name");
		objectData.height = atoi(item->Attribute("height"));
		objectData.width = atoi(item->Attribute("width"));
		objectData.textureSource = item->Attribute("texture");
		objectData.collides = atoi(item->Attribute("collides")) == 1 ? true : false;

		mDataMap[item->Attribute("name")] = objectData;
	}
}
	
ObjectHandler::~ObjectHandler()
{

}

ObjectData ObjectHandler::getData(string name)
{
	// Return the attributes
	return mDataMap[name];
}

map<string, ObjectData> ObjectHandler::getDataMap()
{
	return mDataMap;
}