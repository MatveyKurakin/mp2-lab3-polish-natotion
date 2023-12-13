#include <string>
enum TypeLex {number, binOp, unOp, leftB, rightB, func};

struct Lexema{
	TypeLex type;
	double value;
	std::string name;
	int priority;
};