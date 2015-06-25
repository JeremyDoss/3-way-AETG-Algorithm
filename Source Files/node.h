#include <iostream>

using namespace std;

typedef struct node {
public:
	int x, y, z;
	bool used;

	node();
	bool isUsed();
	void use();
	void reset();
	void print();
};