#include <iostream>
#include <stdio.h>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <set>

#define pii pair<int, int>
#define mp make_pair
#define MAX 512000
#define ss second
#define ff first
#define endl '\n'
#define pb push_back
#define mt make_token

using namespace std;

struct token {
	string type;
	string value;
};

bool debugUno = false, debugDos = true;
char input[MAX + 1];
int currentLine = 1;
int currentColumn = 1;
int endOfFile;
string next_string;
set<char> reserved = {',', ';', '+', '-', '*', '/', '%', '(', ')', '[', ']', '{', '}'};
set<string> reserved_words = {"e", "escreve", "int", "le", "letra", "mainha", "nada", "nao", "ou", "real", "se", "senao", "tome", "uai", "vaza"};
queue<pii> errors;
vector<token> tokens;
map<int, map<string, vector<string> > > table = {
{0, {
{"G", {"0", "0", "1", ""}},
{"T", {"0", "0", "13", ""}},
{"int", {"1", "0", "14", ""}}
}},
{1, {
{"$", {"2", "1", "2", "T"}},
{"int", {"1", "0", "2", ""}}
}},
{2, {
{"identifier", {"1", "0", "3", ""}}
}},
{3, {
{";", {"2", "0", "10", "A"}},
{"=", {"1", "0", "4", ""}},
{"A", {"0", "0", "8", ""}},
{"[", {"1", "0", "10", ""}}
}},
{4, {
{"B", {"0", "0", "5", ""}},
{"identifier", {"1", "0", "6", ""}},
{"numConst", {"1", "0", "7", ""}}
}},
{5, {
{";", {"2", "2", "7", "A"}}
}},
{6, {
{";", {"2", "1", "7", "B"}},
{"]", {"2", "1", "7", "B"}}
}},
{7, {
{";", {"2", "1", "7", "B"}},
{"]", {"2", "1", "7", "B"}}
}},
{8, {
{";", {"1", "0", "9", ""}}
}},
{9, {
{"$", {"2", "5", "9", "G"}},
{"int", {"2", "5", "9", "G"}}
}},
{10, {
{"B", {"0", "0", "11", ""}},
{"identifier", {"1", "0", "6", ""}},
{"numConst", {"1", "0", "7", ""}}
}},
{11, {
{"]", {"1", "0", "12", ""}}
}},
{12, {
{";", {"2", "3", "12", "A"}}
}},
{13, {
{"$", {"3", "0", "12", ""}}
}},
{14, {
{"identifier", {"1", "0", "15", ""}}
}},
{15, {
{";", {"2", "0", "10", "A"}},
{"=", {"1", "0", "4", ""}},
{"A", {"0", "0", "16", ""}},
{"[", {"1", "0", "10", ""}}
}},
{16, {
{";", {"1", "0", "17", ""}}
}},
{17, {
{"$", {"2", "4", "17", "G"}},
{"int", {"2", "4", "17", "G"}}
}}
};

bool input_checker(char* s, bool f);
int next_test(int i);
int test_char(int i);
int test_string(int i);
int test_numConst(int i);
int test_identifier(int i);
int test_eqOp(int i);
int test_gtOp(int i);
int test_ltOp(int i);
int test_df(int i);
token make_token(string type, string value);
bool syntatic();

int main() {
	bool input_flag = false;
	pii p;
	endOfFile = fread(input, sizeof(char), MAX, stdin);
	input[endOfFile] = '\0';
	input_flag = input_checker(input, input_flag);
	if (input_flag) {
		cout << "ARQUIVO INVALIDO\n";
	}
	else {
		int i = 0;
		while (i < endOfFile) {
			i = next_test(i);
		}
		if (errors.empty()) {
			tokens.pb(mt("$", ""));
			if (debugDos) {
				for (token t : tokens) {
					cout << '(' << t.type << ", " << t.value << ") ";
				}
				cout << endl;
			}
			if (syntatic())
				cout << "OK\n";
		}
		else {
			while (!errors.empty()) {
				p = errors.front();
				cout << p.ff << " " << p.ss << endl;
				errors.pop();
			}
		}
	}
}

bool input_checker(char* s, bool input_flag) {
	for (int i = 0; i < endOfFile; i++) {
		if (s[i] < 9 || s[i] > 126 || (s[i] > 10 && s[i] < 32)) {
			input_flag = true;
		}
	}
	return input_flag;
}

int next_test(int i) {
	if (i >= endOfFile) {
		return i;
	}
	else if (input[i] == '\n') {

		if (debugUno) cout << "Enter\n";
		if (debugUno) cout << currentLine << " " << currentColumn << endl;

		currentLine++;
		currentColumn = 1;
		return (i + 1);
	}
	else if (input[i] == ' ' || input[i] == '	') {

		if (debugUno && input[i] == ' ') cout << "Espaço\n";
		if (debugUno && input[i] == '	') cout << "Espaço\n";
		if (debugUno) cout << currentLine << " " << currentColumn << endl;

		currentColumn++;
		return (i + 1);
	}
	else if (reserved.count(input[i])) {
		next_string = input[i];
		tokens.pb(mt(next_string, ""));
		currentColumn++;
		return (i + 1);
	}
	else if (input[i] == '\'') {
		return test_char(i);
	}
	else if (input[i] == '"') {
		return test_string(i);
	}
	else if (input[i] >= '0' && input[i] <= '9') {
		return test_numConst(i);
	}
	else if (input[i] == '_' || (input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z')) {
		return test_identifier(i);
	}
	else if (input[i] == '=') {
		return test_eqOp(i);
	}
	else if (input[i] == '>') {
		return test_gtOp(i);
	}
	else if (input[i] == '<') {
		return test_ltOp(i);
	}
	else if (input[i] == '!') {
		return test_df(i);
	}
	else {
		errors.push(mp(currentLine, currentColumn));
		currentColumn++;
		return (i + 1);
	}
}

int test_char(int i) {

	if (debugUno) cout << "Testando caractere\n";
	if (debugUno) cout << currentLine << " " << currentColumn << endl;

	i++;
	currentColumn++;
	if (input[i] == '\0') {
		errors.push(mp(currentLine, currentColumn - 1));
	}
	else if (input[i] == '\'') {
		errors.push(mp(currentLine, currentColumn - 1));
		return i;
	}
	else if (input[i] == '\n') {
		if (input[i + 1] == '\'') {
			tokens.pb(mt("char", "\n"));
			currentLine++;
			currentColumn = 2;
			return (i + 2);
		}
		else {
			errors.push(mp(currentLine, currentColumn));
			currentLine++;
			currentColumn = 1;
			return (i + 1);
		}
	}
	else {
		if (input[i + 1] == '\'') {
			next_string = input[i];
			tokens.pb(mt("char", next_string));
			currentColumn += 2;
			return (i + 2);
		}
		else {
			errors.push(mp(currentLine, currentColumn));
			currentColumn++;
			return (i + 1);
		}
	}
	return i;
}

int test_string(int i) {

	if (debugUno) cout << "Testando string\n";
	if (debugUno) cout << currentLine << " " << currentColumn << endl;

	int size = 0;
	int prevL, prevC;
	next_string = "";
	i++;
	currentColumn++;
	while (size < 128) {
		if (input[i] == '\0') {
			if (debugUno) cout << "INVALIDO\n";
			errors.push(mp(currentLine, currentColumn - 1));
			return i;
		}
		else if (input[i] == '\n') {
			next_string += input[i];
			if (input[i + 1] == '\0') {
				errors.push(mp(currentLine, currentColumn));
				return (i + 1);
			}
			else {
				prevC = currentColumn;
				prevL = currentLine;
				size++;
				i++;
				currentLine++;
				currentColumn = 1;
			}
		}
		else if (input[i] == '"') {
			tokens.pb(mt("string", next_string));
			if (debugUno) cout << "VALIDO\n";
			currentColumn++;
			return (i + 1);
		}
		else {
			next_string += input[i];
			size++;
			i++;
			currentColumn++;
		}
	}
	if (input[i] != '"') {
		if (debugUno) cout << "INVALIDO\n";
		if (size > 0 && input[i - 1] == '\n') {
			errors.push(mp(prevL, prevC));
		}
		else {
			errors.push(mp(currentLine, currentColumn - 1));
		}
		return i;
	}
	else {
		tokens.pb(mt("string", next_string));
		if (debugUno) cout << "VALIDO\n";
		currentColumn++;
		return (i + 1);
	}
}

int test_numConst(int i) {

	if (debugUno) cout << "Testando constante numerica\n";
	if (debugUno) cout << currentLine << " " << currentColumn << endl;

	bool real = false;
	bool fail = false;
	int size = 1;
	next_string = input[i];
	i++;
	currentColumn++;
	while (size < 128 && ((input[i] >= '0' && input[i] <= '9') || input[i] == '.')) {
		next_string += input[i];
		if (input[i] == '.') {
			if(!real) {
				real = true;
			}
			else {
				fail = true;
				break;
			}
		}
		size++;
		i++;
		currentColumn++;
	}
	if ((size == 128 && ((input[i] >= '0' && input[i] <= '9') || input[i] == '.')) || fail ||
						 (input[i] == '_' ||
						 (input[i] >= 'A' && input[i] <= 'Z') ||
						 (input[i] >= 'a' && input[i] <= 'z'))) {
		errors.push(mp(currentLine, currentColumn - 1));
		if (debugUno) cout << "INVALIDO\n";
	}
	else {
		tokens.pb(mt("numConst", next_string));
		if (debugUno) cout << "VALIDO\n";
	}
	return i;

}

int test_identifier(int i) {

	if (debugUno) cout << "Testando identificador\n";
	if (debugUno) cout << currentLine << " " << currentColumn << endl;

	int size = 1;
	next_string = input[i];
	i++;
	currentColumn++;
	while (size < 128 && (input[i] == '_' ||
						 (input[i] >= 'A' && input[i] <= 'Z') ||
						 (input[i] >= 'a' && input[i] <= 'z') ||
						 (input[i] >= '0' && input[i] <= '9'))) {
		next_string += input[i];
		size++;
		i++;
		currentColumn++;
	}
	if (size == 128 && (input[i] == '_' ||
					   (input[i] >= 'A' && input[i] <= 'Z') ||
					   (input[i] >= 'a' && input[i] <= 'z') ||
					   (input[i] >= '0' && input[i] <= '9'))) {
		if (debugUno) cout << "INVALIDO\n";
		errors.push(mp(currentLine, currentColumn - 1));
	}
	else {
		if (reserved_words.count(next_string)) {
			tokens.pb(mt(next_string, ""));
		}
		else {
		tokens.pb(mt("identifier", next_string));
		}
		if (debugUno) cout << "VALIDO\n";
	}
	return i;
}

int test_eqOp(int i) {

	if (debugUno) cout << "Testando igual\n";
	if (debugUno) cout << currentLine << " " << currentColumn << endl;

	if(input[i + 1] == '=') {
		if (debugUno) cout << "VALIDO\n";
		tokens.pb(mt("==", ""));
		currentColumn += 2;
		return (i + 2);
	}
	else {
		if (debugUno) cout << "VALIDO\n";
		tokens.pb(mt("=", ""));
		currentColumn++;
		return (i + 1);
	}
}

int test_gtOp(int i) {

	if (debugUno) cout << "Testando maior\n";
	if (debugUno) cout << currentLine << " " << currentColumn << endl;

	if(input[i + 1] == '=') {
		if (debugUno) cout << "VALIDO\n";
		tokens.pb(mt(">=", ""));
		currentColumn += 2;
		return (i + 2);
	}
	else {
		if (debugUno) cout << "VALIDO\n";
		tokens.pb(mt(">", ""));
		currentColumn++;
		return (i + 1);
	}
}

int test_ltOp(int i) {

	if (debugUno) cout << "Testando menor\n";
	if (debugUno) cout << currentLine << " " << currentColumn << endl;

	if(input[i + 1] == '=') {
		if (debugUno) cout << "VALIDO\n";
		tokens.pb(mt("<=", ""));
		currentColumn += 2;
		return (i + 2);
	}
	else {
		if (debugUno) cout << "VALIDO\n";
		tokens.pb(mt("<", ""));
		currentColumn++;
		return (i + 1);
	}
}

int test_df(int i) {

	if (debugUno) cout << "Testando diferente\n";
	if (debugUno) cout << currentLine << " " << currentColumn << endl;

	if(input[i + 1] == '=') {
		if (debugUno) cout << "VALIDO\n";
		tokens.pb(mt("!=", ""));
		currentColumn += 2;
		return (i + 2);
	}
	else {
		if (debugUno) cout << "INVALIDO\n";
		errors.push(mp(currentLine, currentColumn));
		currentColumn++;
		return (i + 1);
	}
}

token make_token(string type, string value) {
	token created;
	created.type = type;
	created.value = value;
	return created;
}

bool syntatic() {
	stack<string> slr;
	slr.push("0");
	map<string, vector<string> > current_state;
	string r_head;
	vector<token>::iterator it = tokens.begin();
	while (1) {
		current_state = table[stoi(slr.top())];
		if (current_state.count(it->type)) {
			int prev_state, counter;
			if (debugDos) cout << slr.top() << endl;
			switch (stoi(current_state[it->type][0])) {
				case 0:
					if (debugDos) cout << "ZERO " << it->type << endl;
					break;
				case 1:
					if (debugDos) cout << "UM " << it->type << endl;
					slr.push(it->type);
					slr.push(current_state[it->type][2]);
					it++;
					break;
				case 2:
					counter = stoi(current_state[it->type][1]);
					counter *= 2;
					cout << "counter " << counter << endl;
					while (counter--) {
						//botar num node
						slr.pop();
					}
					prev_state = stoi(slr.top());
					r_head = current_state[it->type][3];
					slr.push(r_head);
					current_state = table[prev_state];
					cout << r_head << " " << prev_state << endl;
					slr.push(current_state[r_head][2]);
					if (debugDos) cout << "DOIS " << slr.size() << it->type << endl;
					break;
				case 3:
					counter = stoi(current_state[it->type][1]);
					counter *= 2;
					if (debugDos) cout << "TRÊS " << it->type << endl;
					while (counter--) {
						//botar num node
						slr.pop();
					}
					return true;
					break;
			} 
		}
		else {
			if (debugDos) cout << slr.top() << endl;
			return false;
		}
	}
}