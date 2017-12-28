#ifndef GPIO_H
#define GPIO_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
using namespace std;


class GPIO
{
public:
	GPIO(string pin_num);
	GPIO();
	GPIO(string pin_num, string dir);
	~GPIO();
	void set(void);
	void reset(void);
	int get(void);
	
private:
	string _pin_number;
	string _direction;
	ofstream exxport;
	ofstream direction;
	ofstream out_value;
	ifstream in_value;

};



#endif // !GPIO_H
