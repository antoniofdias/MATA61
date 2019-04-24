#include <iostream>
#include <stdio.h>
#include <utility>
#include <string>
#include <queue>
#include <set>

#define pii pair<int, int>
#define mp make_pair
#define MAX 512000
#define ss second
#define ff first
#define endl '\n'
#define debug 0

using namespace std;

char input[MAX + 1];
int currentLine = 1;
int currentColumn = 1;
//int i;
int endOfFile;
set<char> reserved = {',', ';', '+', '-', '*', '/', '%', '(', ')', '[', ']', '{', '}'};
set<string> reserved_words = {"e", "escreve", "int", "le", "letra", "mainha", "nada", "nao", "ou", "real", "se", "senao", "tome", "uai", "vaza"};
queue<pii> errors;
pii p;

bool input_checker(char* s, bool f);
void next_token(int i);
void test_char(int i);
void test_string(int i);
void test_numConst(int i);
void test_identifier(int i);
void test_eqOp(int i);
void test_gtOp(int i);
void test_ltOp(int i);
void test_df(int i);

int main() {
	bool input_flag = false;
	endOfFile = fread(input, sizeof(char), MAX, stdin);
	input[endOfFile] = '\0';
	input_flag = input_checker(input, input_flag);
	if (input_flag) {
		cout << "ARQUIVO INVALIDO\n";
	}
	else {
		next_token(0);
		if (errors.empty()) {
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

void next_token(int i) {
	if (i >= endOfFile) return;
	else if (input[i] == '\n') {
		currentLine++;
		currentColumn = 1;
		next_token(i+1);
	}
	else if (input[i] == ' ' || input[i] == '	') {
		currentColumn++;
		next_token(i+1);
	}
	else if (reserved.count(input[i])) {
		// Lembrar de gerar tokens
		next_token(i+1);
	}
	else if (input[i] == '\'') {
		test_char(i);
	}
	else if (input[i] == '"') {
		test_string(i);
		
	}
	else if (input[i] >= '0' && input[i] <= '9') {
		test_numConst(i);
	}
	else if (input[i] == '_' || (input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z')) {
		test_identifier(i);
	}
	else if (input[i] == '=') {
		test_eqOp(i);
	}
	else if (input[i] == '>') {
		test_gtOp(i);
	}
	else if (input[i] == '<') {
		test_ltOp(i);
	}
	else if (input[i] == '!') {
		test_df(i);
	}
	else {
		errors.push(mp(currentLine, currentColumn));
		next_token(i+1);
	}
}

void test_char(int i) {

	if (debug) cout << "Testando caractere\n";
	if (debug) cout << currentLine << " " << currentColumn << endl;

	if (input[i + 1] == '\'') {
		if (debug) cout << "INVÁLIDO\n";
		errors.push(mp(currentLine, currentColumn));
		currentColumn++;
		next_token(i + 1);
	}
	else if ((i + 2) < endOfFile && input[i + 2] == '\'') {
		if (debug) cout << "VÁLIDO\n";
		// Gera token char
		currentColumn += 3;
		next_token(i + 3);
	}
	else if ((i + 1) == endOfFile) {
		if (debug) cout << "INVÁLIDO\n";
		errors.push(mp(currentLine, currentColumn));
		return;
	}
	else if ((i + 2) == endOfFile) {
		if (debug) cout << "INVÁLIDO\n";
		errors.push(mp(currentLine, currentColumn + 1));
		return;
	}
	else {
		if (debug) cout << "INVÁLIDO\n";
		errors.push(mp(currentLine, ++currentColumn));
		currentColumn++;
		next_token(i+2);
	}
}

void test_string(int i) {

}

void test_numConst(int i) {

}

void test_identifier(int i) {

}

void test_eqOp(int i) {

	if (debug) cout << "Testando diferente\n";
	if (debug) cout << currentLine << " " << currentColumn << endl;

	if(input[i + 1] == '=') {
		if (debug) cout << "VÁLIDO\n";
		// Gera token igual lógico
		currentColumn += 2;
		next_token(i + 2);
	}
	else {
		if (debug) cout << "INVÁLIDO\n";
		// Gera token igual aritmético
		currentColumn++;
		next_token(i + 1);
	}
}

void test_gtOp(int i) {

}

void test_ltOp(int i) {

}

void test_df(int i) {

	if (debug) cout << "Testando diferente\n";
	if (debug) cout << currentLine << " " << currentColumn << endl;

	if(input[i + 1] == '=') {
		if (debug) cout << "VÁLIDO\n";
		// Gera token diferente
		currentColumn += 2;
		next_token(i + 2);
	}
	else {
		if (debug) cout << "INVÁLIDO\n";
		errors.push(mp(currentLine, currentColumn));
		currentColumn++;
		next_token(i + 1);
	}
}
