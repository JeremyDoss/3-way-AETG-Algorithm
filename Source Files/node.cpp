#include "node.h"

node::node() {
	x = -1;
	y = -1;
	z = -1;
	used = false;
}

bool node::isUsed() {
	return used;
}

void node::use() {
	used = true;
}

void node::reset() {
	used = false;
}

void node::print() {
	cout << "(" << x << ", " << y << ", " << z << ")";
}