#ifndef WMESSAGE_H
#define WMESSAGE_H

#include <string>

using namespace std;

// Each window component sends a wMessage to it's callback function
// :NOTE: Maybe not the best solution but it works!
class wMessage
{
public:
	wMessage(int data);
	wMessage(string data);
	wMessage(float data);
	wMessage(bool data);
	~wMessage();

	int		getInt(void);
	float	getFloat(void);
	string	getString(void);
	bool	getBool(void);
private:
	// Each message gets stored as a string, use the right get() function above to get it to the data type you want
	string mData;
};

#endif