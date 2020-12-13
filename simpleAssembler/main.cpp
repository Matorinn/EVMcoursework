#include <iostream>
#include <regex>
#include <fstream>

using namespace std;

#define READ 0x10
#define WRITE 0x11
#define LOAD 0x20
#define STORE 0x21
#define JUMP 0x40
#define JNEG 0x41
#define JZ 0x42
#define HALT 0x43
#define ADD 0x30
#define SUB 0x31
#define DIVIDE 0x32
#define MUL 0x33
#define NEG 0x64

int main(int argc, char *argv[]) {
    if (argc != 3)// если передаем аргументы, то argc будет больше 1(в зависимости от кол-ва аргументов)
    {
        return -1;
    }
    regex sa(R"(\w+\.sa)");
    regex o(R"(\w+\.o)");
    if (!regex_match((string) argv[1], sa)) {
        cout << "НЕВЕРНЫЙ ВВОД" << endl;
    }
    if (!regex_match((string) argv[2], o)) {
        cout << "НЕВЕРНЫЙ ВВОД" << endl;
    }
    ifstream saFile(argv[1]);
    short memory[100];
    for (int i = 0; i < 100; i++)
        memory[i] = 0;
    string lineToString;
    while (getline(saFile, lineToString)) {
        if (lineToString.find(";") != -1) {//стирает коментарий, если он есть
            lineToString.erase(lineToString.find(";"));
        }
        std::istringstream input(lineToString);
        std::vector<std::string> result;
        while (!input.eof()) {
            string substring;
            input >> substring;
            result.push_back(substring);
        }
        if ((result.size() != 3 && result.size() != 4)) { // если строка не соответствует формату
            cout << "СТРОКА НЕ СООТВЕТСТВУЕТ ФОРМАТУ" << endl;
            return -1;
        }
        if (result[0].size() != 2) {
            cout << "НЕВЕРНЫЙ АДРЕС ЯЧЕЙКИ ПАМЯТИ" << endl;
            return -1;
        }
        if ((result[2].size() != 2) && (result[2].size() != 5) && (result[2].size() != 4)) {
            cout << "НЕВЕРНЫЙ ОПЕРАНД" << endl;
            return -1;
        }
        regex num(R"(\d{2})");
        if (!regex_match(result[0], num)) {
            cout << "НЕВЕРНЫЙ АДРЕС ЯЧЕЙКИ ПАМЯТИ" << endl;
        }
        regex oper;
        if (result[1] == "=") {
            oper = (R"(\-?\d{4})");
        } else {
            oper = (R"(\d{2})");
        }
        if (!regex_match(result[2], oper)) {
            cout << "НЕВЕРНЫЙ ОПЕРАНД !!" << endl;
        }
        short command;
        short numb;
        short operand;
        if (result[1] == "READ") {
            command = READ;
        } else if (result[1] == "WRITE") {
            command = WRITE;
        } else if (result[1] == "LOAD") {
            command = LOAD;
        } else if (result[1] == "STORE") {
            command = STORE;
        } else if (result[1] == "JUMP") {
            command = JUMP;
        } else if (result[1] == "JNEG") {
            command = JNEG;
        } else if (result[1] == "JZ") {
            command = JZ;
        } else if (result[1] == "HALT") {
            command = HALT;
        } else if (result[1] == "ADD") {
            command = ADD;
        } else if (result[1] == "SUB") {
            command = SUB;
        } else if (result[1] == "DIVIDE") {
            command = DIVIDE;
        } else if (result[1] == "MUL") {
            command = MUL;
        } else if (result[1] == "NEG") {
            command = NEG;
        } else if (result[1] == "=") {
            numb = (short) stoi(result[0]);
            regex pos(R"([0-9A-Fa-f]{4})");
            regex neg(R"([-]{1}[0-9A-Fa-f]{4})");
            if (regex_match(result[2], pos)) {
                operand = (short) stoi(result[2], nullptr, 16);
                operand |= (1 << 14);
                memory[numb] = operand;
            } else if (regex_match(result[2], neg)) {
                operand = (short) stoi(result[2], nullptr, 16);
                operand = ~(operand - 1);//стало просто числом
                operand |= (1 << 14); // стало не командой внутри системы СК
                operand ^= 0b11111111111111;
                operand += 1;
                memory[numb] = operand;
            } else {
                cout << "ВВЕДЕНО НЕВЕРНОЕ ЧИСЛО" << endl;
                return -1;
            }
            continue;
        } else {
            cout << "НЕВЕРНАЯ КОМАНДА" << endl;
            return -1;
        }
        numb = (short) stoi(result[0]);
        operand = (short) stoi(result[2]);
        short value;
        if (operand < 0 || operand >= 100) {
            return -1;
        }
        value = 128;
        value *= command;
        value += operand;
        memory[numb] = value;
    }
    FILE *op;
    if ((op = fopen(argv[2], "wb")) == NULL) {
        cout << "ФАЙЛА С ДАННЫМ НАЗВАНИЕМ НЕ СУЩЕСТВУЕТ 2" << endl;
        return -1;
    }
    fwrite(memory, sizeof(memory), 1, op);
    fclose(op);
    return 0;
}