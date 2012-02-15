#include "wMessage.h"
#include <boost\lexical_cast.hpp>

/*
	The following funtions makes the class wMessage easy to use
	If the message you recieved is suppose to be a int just call getInt()
	boost::lexical_cast<T> casts the mData member to the type you want it to have
*/

wMessage::wMessage(string data)	
{
	mData = boost::lexical_cast<string>(data);
}

wMessage::wMessage(int data)	
{
	mData = boost::lexical_cast<string>(data);
}

wMessage::wMessage(float data)	
{
	mData = boost::lexical_cast<string>(data);
}

wMessage::wMessage(bool data)	
{
	if(data)
		mData = "true";
	else
		mData = "false";
}

wMessage::~wMessage() 
{
	// Dtor
}

std::string wMessage::getString(void)
{
	return mData;
}

int wMessage::getInt(void)
{
	return boost::lexical_cast<int>(mData);
}
	
float wMessage::getFloat(void)
{
	return boost::lexical_cast<float>(mData);
}

bool wMessage::getBool(void)
{
	if(mData == "true")
		return true;
	else
		return false;
}