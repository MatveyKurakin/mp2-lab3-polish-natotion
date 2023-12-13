#include "lexema.h"
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

enum class Status {wait, numberSt, binOp, unOp, leftB, func};

int countBoard=0;

Lexema getLexems(string input, int pos, int& nextPos)
{
	Lexema l;
	if (input[pos] - '0' >= 0 && input[pos] - '0' <= 9)
	{
		double num = 0;
		while (input[pos] - '0' >= 0 && input[pos] - '0' <= 9 && pos < input.size())
		{
			num *= 10;
			num += input[pos] - '0';
			pos++;
		}
		if (input[pos] == '.') 
		{
			double cnt = 0.1;
			pos++;
			while (input[pos] - '0' >= 0 && input[pos] - '0' <= 9 && pos < input.size())
			{
				num += (input[pos] - '0') * cnt;
				cnt *= 0.1;
				pos++;
			}
		}
		nextPos = pos;
		l.type = TypeLex::number;
		l.value = num;
	}
	else if (input[pos] == '+')
	{
		nextPos = pos + 1;
		l.type = TypeLex::binOp;
		l.name = "+";
		l.priority = 0;
	}
	else if (input[pos] == '-')
	{
		if (pos == 0 || input[pos - 1] == '(') {
			nextPos = pos + 1;
			l.type = TypeLex::unOp;
			l.name = "-";
		}
		else
		{
			nextPos = pos + 1;
			l.type = TypeLex::binOp;
			l.name = "-";
			l.priority = 1;
		}
	}
	else if (input[pos] == '*')
	{
		nextPos = pos + 1;
		l.type = TypeLex::binOp;
		l.name = "*";
		l.priority = 2;
	}
	else if (input[pos] == '/')
	{
		nextPos = pos + 1;
		l.type = TypeLex::binOp;
		l.name = "/";
		l.priority = 2;
	}
	else if (input[pos] == '^')
	{
		nextPos = pos + 1;
		l.type = TypeLex::binOp;
		l.name = "^";
		l.priority = 3;
	}
	else if (input[pos] == '(')
	{
		nextPos = pos + 1;
		l.type = TypeLex::leftB;
		//countBoard++;
	}
	else if (input[pos] == ')')
	{
		nextPos = pos + 1;
		l.type = TypeLex::rightB;
		//countBoard--;
		//if (countBoard < 0) throw("error");
	}
	else if (pos + 2 < input.size() && input[pos] == 's' && input[pos + 1] == 'i' && input[pos + 2] == 'n')
	{
		nextPos = pos + 3;
		l.type = TypeLex::func;
		l.name = "sin";
		l.priority = 3;
	}
	else if (pos + 2 < input.size() && input[pos] == 'c' && input[pos + 1] == 'o' && input[pos + 2] == 's')
	{
		nextPos = pos + 3;
		l.type = TypeLex::func;
		l.name = "cos";
		l.priority = 3;
	}
	else if (pos + 1 < input.size() && input[pos] == 't' && input[pos + 1] == 'g')
	{
		nextPos = pos + 2;
		l.type = TypeLex::func;
		l.name = "tg";
		l.priority = 3;
	}
	else if (pos + 2 < input.size() && input[pos] == 'c' && input[pos + 1] == 't' && input[pos + 2] == 'g')
	{
		nextPos = pos + 3;
		l.type = TypeLex::func;
		l.name = "ctg";
		l.priority = 3;
	}
	else if (pos + 1 < input.size() && input[pos] == 'l' && input[pos + 1] == 'n')
	{
		nextPos = pos + 2;
		l.type = TypeLex::func;
		l.name = "ln";
		l.priority = 3;
	}
	else if (pos + 2 < input.size() && input[pos] == 'l' && input[pos + 1] == 'g')
	{
		pos += 2;
		l.type = TypeLex::func;
		l.name = "lg";
		l.priority = 3;
		int num = 0;
		while (input[pos] - '0' >= 0 && input[pos] - '0' <= 9 && pos < input.size())
		{
			num *= 10;
			num += input[pos] - '0';
			pos++;
		}
		nextPos = pos;
		l.value = num;
	}
	else if (input[pos] == 'e')
	{
		nextPos = pos + 1;
		l.type = TypeLex::number;
		l.value = exp(1);
	}
	else if (pos + 1 < input.size() && input[pos] == 'P' && input[pos + 1] == 'I')
	{
		nextPos = pos + 2;
		l.type = TypeLex::number;
		l.value = M_PI;
		}
	else
	{
		throw("errorName");
	}
	return l;
}

void checkInput(vector<Lexema> lexems) {
	Status currentSt = Status::wait;

	for (int i = 0; i < lexems.size(); i++) {
		TypeLex type = lexems[i].type;
		if (currentSt == Status::wait) {
			if (type == TypeLex::number) currentSt = Status::numberSt;
			if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
			if (type == TypeLex::rightB) throw("errorRB");
			if (type == TypeLex::binOp) throw("errorBinOp");
			if (type == TypeLex::unOp) currentSt = Status::wait;
			if (type == TypeLex::func) currentSt = Status::func;
		}
		else if (currentSt == Status::numberSt) {
			if (type == TypeLex::binOp) currentSt = Status::binOp;
			if (type == TypeLex::rightB) { currentSt = Status::numberSt; countBoard--; }
			if (type == TypeLex::leftB) throw("errorLB");
			if (type == TypeLex::func) throw("errorFunc");
		}
		else if (currentSt == Status::binOp) {
			if (type == TypeLex::number) currentSt = Status::numberSt;
			if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
			if (type == TypeLex::rightB) throw("errorRB");
			if (type == TypeLex::binOp) throw("errorBinOp");
			if (type == TypeLex::func) currentSt = Status::wait;
		}
		else if (currentSt == Status::leftB) {
			if (type == TypeLex::number) currentSt = Status::numberSt;
			if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
			if (type == TypeLex::rightB) throw("errorB");
			if (type == TypeLex::binOp) throw("errorBinOp");
			if (type == TypeLex::unOp) currentSt = Status::wait;
			if (type == TypeLex::func) currentSt = Status::wait;
		}
		else if (currentSt == Status::func) {
			if (type == TypeLex::number) throw("errorNumber");
			if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
			if (type == TypeLex::rightB) throw("errorRB");
			if (type == TypeLex::binOp) throw("errorBinOp");
			if (type == TypeLex::unOp) throw("errorUnOp");
			if (type == TypeLex::func) throw("error");
		}

		if (countBoard < 0) throw("errorCountB-");
	}
	if (countBoard != 0) throw("errorCountB+");
}

double getBinOperation(double x1, double x2, Lexema l) 
{
	if (l.name == "*") return x1 * x2;
	if (l.name == "/") return x1 / x2;
	if (l.name == "+") return x1 + x2;
	if (l.name == "-") return x1 - x2;
	if (l.name == "^") return pow(x1,x2);
}

double getFunc(double x, Lexema l) 
{
	if (l.name == "sin") {
		return sin(x);
	}
	if (l.name == "cos") {
		return cos(x);
	}
	if (l.name == "tg") {
		return tan(x);
	}
	if (l.name == "ctg") {
		return pow(tan(x), -1);
	}
	if (l.name == "ln") {
		return log(x);
	}
	if (l.name == "lg") {
		return log(x) / log(l.value);
	}
}

double countingLexems(vector<Lexema> lexems) {
	queue<Lexema> pExpression;
	stack<Lexema> operation;
	for (int i = 0; i < lexems.size(); i++)
	{
		if (lexems[i].type == TypeLex::number) pExpression.push(lexems[i]);
		if (lexems[i].type == TypeLex::binOp) 
		{
			if (!operation.empty() && operation.top().priority > lexems[i].priority)
			{
				while (!operation.empty() && operation.top().type != TypeLex::leftB)
				{
					pExpression.push(operation.top());
					operation.pop();
				}
			}
			operation.push(lexems[i]);
		}
		if (lexems[i].type == TypeLex::unOp) 
		{
			Lexema l;
			l.type = TypeLex::number;
			l.value = -1;
			pExpression.push(l);
			l.type = TypeLex::binOp;
			l.name = "*";
			l.priority = 2;
			operation.push(l);
		}
		if (lexems[i].type == TypeLex::leftB) operation.push(lexems[i]);
		if (lexems[i].type == TypeLex::rightB)
		{
			while (operation.top().type != TypeLex::leftB)
			{
				pExpression.push(operation.top());
				operation.pop();
			}
			operation.pop();
			if (!operation.empty() && operation.top().type == TypeLex::func)
			{
				pExpression.push(operation.top());
				operation.pop();
			}
		}
		if (lexems[i].type == TypeLex::func) operation.push(lexems[i]);
	}
	while (!operation.empty())
	{
		pExpression.push(operation.top());
		operation.pop();
	}
	stack<double> result;
	while (!pExpression.empty()) {
		if (pExpression.front().type == TypeLex::number) result.push((double)pExpression.front().value);
		else if (pExpression.front().type == TypeLex::binOp)
		{
			double x1 = result.top();
			result.pop();
			double x2 = result.top();
			result.pop();
			result.push(getBinOperation(x2, x1, pExpression.front()));
		}
		else if (pExpression.front().type == TypeLex::func)
		{
			double x = result.top();
			result.pop();
			result.push(getFunc(x, pExpression.front()));
		}
		pExpression.pop();
	}
	return result.top();
}

int main() {
	string input;
	cin >> input;
	int startPos = 0;
	int endPos = 0;

	vector<Lexema> lexems;

	try {
		while (endPos < input.size())
		{
			lexems.push_back(getLexems(input, startPos, endPos));
			startPos = endPos;
		}

		checkInput(lexems);
	}
	catch (const char* a) {
		cout << a;
		return 0;
	}
	
	double result = countingLexems(lexems);
	cout << result;

	return 0;
}