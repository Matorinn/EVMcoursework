#include <iostream>
#include <regex>
#include <fstream>
#include <iomanip>

using namespace std;

short cellvar = 99; //ячейки для переменных и цифр
short cellcom = 0;

struct variable {
    variable(string name, short address) {
        this->address = address;
        this->name = name;
    }
    string name;
    short address;
};

struct commandSA {
    commandSA(short cell, const string &command, short operand) {
        this->cell = cell;
        this->command = command;
        this->operand = operand;
    }
    short cell;
    string command;
    short operand;
};

map<string, short> commandStart; // строка и адрес в SC
vector<variable> variables;
vector<commandSA> commandVector;
vector<short> gotoQueue;

const regex simplecheck(R"((\d+)\s+(REM|INPUT|PRINT|LET|GOTO|IF|END)\s*(.*))");
const regex varcheck(R"([A-Z]{1}\s*)");
const regex numcheck(R"(\-?\d+\s*)");
const regex letonecheck(R"(([A-Z])\s+=\s+([A-Z]|-?\d+)\s*)");
const regex lettwocheck(R"(([A-Z])\s+=\s+([A-Z]|-?\d+)\s+([\+\-\*\/])\s+([A-Z]|-?\d+)\s*)");
const regex ifcheck(R"(([A-Z]|-?\d+)\s*(>|<|=)\s*([A-Z]|-?\d+)\s+(REM|INPUT|PRINT|LET|GOTO|END)\s*(.*))");

int findfun(string str) {
    for (int i = 0; i < variables.size(); i++) {
        if (variables[i].name == str)
            return i;
    }
    return -1;
}

int inputFun(string var) {
    if (!regex_match(var, varcheck)) {
        cout << "НЕВЕРНАЯ ПЕРЕМЕННАЯ ПРИ ВВОДЕ" << endl;
        exit(-1);
    }
    short operand = findfun(var);
    if (operand == -1) {
        variables.push_back(variable(var, cellvar));
        cellvar--;
        operand = variables.size() - 1;
    }
    commandVector.push_back(commandSA(cellcom, "READ", variables[operand].address));
    cellcom++;
    return 0;
}

int printFun(string var) {
    if (!regex_match(var, varcheck) && !regex_match(var, numcheck)) {
        cout << "НЕВЕРНАЯ ПЕРЕМЕННАЯ/ЧИСЛО ПРИ ВВОДЕ" << endl;
        exit(-1);
    }
    short operand = findfun(var);
    if (operand == -1) {
        variables.push_back(variable(var, cellvar));
        cellvar--;
        operand = variables.size() - 1;
    }
    commandVector.push_back(commandSA(cellcom, "WRITE", variables[operand].address));
    cellcom++;
    return 0;
}

int gotoFun(string row) {
    if (!regex_match(row, numcheck)) {
        cout << "НЕВЕРНАЯ КОМАНДА ГОТУ" << endl;
        exit(-1);
    }
    if (commandStart.find(row) == commandStart.end()) {
        commandVector.push_back(commandSA(cellcom, "JUMP", stoi(row)));
        gotoQueue.push_back(commandVector.size() - 1);
    } else {
        short operand = commandStart.find(row)->second;
        commandVector.push_back(commandSA(cellcom, "JUMP", operand));
    }
    cellcom++;
    return 0;
}

int letFun(string logic, cmatch letOneLine, cmatch letTwoLine) {
    if (!regex_match(logic.c_str(), letOneLine, letonecheck) && !regex_match(logic.c_str(), letTwoLine, lettwocheck)) {
        cout << "НЕВЕРНАЯ КОМАНДА ЛЕТ" << endl;
        exit(-1);
    }
    if (regex_match(logic, letonecheck)) {
        string var = letOneLine[1];
        string value = letOneLine[2];
        short valueAddress = findfun(value);
        if (valueAddress == -1) {
            variables.push_back(variable(value, cellvar));
            cellvar--;
            valueAddress = variables.size() - 1;
        }
        commandVector.push_back(commandSA(cellcom, "LOAD", variables[valueAddress].address));
        cellcom++;

        short varAddress = findfun(var);
        if (varAddress == -1) {
            variables.push_back(variable(var, cellvar));
            cellvar--;
            varAddress = variables.size() - 1;
        }
        commandVector.push_back(commandSA(cellcom, "STORE", variables[varAddress].address));
        cellcom++;
    } else {
        string var = letTwoLine[1];
        string value1 = letTwoLine[2];
        string sign = letTwoLine[3];
        string value2 = letTwoLine[4];
        /////////////////////////////////////// value1
        short valueAddress1 = findfun(value1);
        if (valueAddress1 == -1) {
            variables.push_back(variable(value1, cellvar));
            cellvar--;
            valueAddress1 = variables.size() - 1;
        }
        commandVector.push_back(commandSA(cellcom, "LOAD", variables[valueAddress1].address));
        cellcom++;
        /////////////////////////////////////// value2
        short valueAddress2 = findfun(value2);
        if (valueAddress2 == -1) {
            variables.push_back(variable(value2, cellvar));
            cellvar--;
            valueAddress2 = variables.size() - 1;
        }
        if (sign == "+") {
            commandVector.push_back(commandSA(cellcom, "ADD", variables[valueAddress2].address));
        } else if (sign == "-") {
            commandVector.push_back(commandSA(cellcom, "SUB", variables[valueAddress2].address));
        } else if (sign == "*") {
            commandVector.push_back(commandSA(cellcom, "MUL", variables[valueAddress2].address));
        } else if (sign == "/") {
            commandVector.push_back(commandSA(cellcom, "DIVIDE", variables[valueAddress2].address));
        }
        cellcom++;
        //////////////////////////////////////// var
        short varAddress = findfun(var);
        if (varAddress == -1) {
            variables.push_back(variable(var, cellvar));
            cellvar--;
            varAddress = variables.size() - 1;
        }
        commandVector.push_back(commandSA(cellcom, "STORE", variables[varAddress].address));
        cellcom++;
    }
    return 0;
}

int endFun(string rub) {
    if (rub != "") {
        cout << "НЕВЕРНАЯ КОМАНДА ЭНД" << endl;
        exit(-1);
    }
    commandVector.push_back(commandSA(cellcom, "HALT", 0));
    cellcom++;
    return 0;
}

int ifFun(string logic) {
    cmatch parsedifline;
    int jumpboy;
    if (!regex_match(logic.c_str(), parsedifline, ifcheck)) {
        cout << "НЕВЕРНАЯ КОМАНДА ИФ" << logic << endl;
        exit(-1);
    }
    short operand1 = findfun(parsedifline[1]);
    if (operand1 == -1) {
        variables.push_back(variable(parsedifline[1], cellvar));
        cellvar--;
        operand1 = variables.size() - 1;
    }
    short operand2 = findfun(parsedifline[3]);
    if (operand2 == -1) {
        variables.push_back(variable(parsedifline[3], cellvar));
        cellvar--;
        operand2 = variables.size() - 1;
    }
    if (parsedifline[2] == "=") {
        commandVector.push_back(commandSA(cellcom, "LOAD", variables[operand2].address));
        cellcom++;
        commandVector.push_back(commandSA(cellcom, "SUB", variables[operand1].address));
        cellcom++;
        commandVector.push_back(commandSA(cellcom, "JZ", cellcom + 2));
        cellcom++;
        commandVector.push_back(commandSA(cellcom, "JUMP", -1));
        jumpboy = commandVector.size() - 1;
        cellcom++;
    } else if (parsedifline[2] == ">") {
        commandVector.push_back(commandSA(cellcom, "LOAD", variables[operand2].address));
        cellcom++;
        commandVector.push_back(commandSA(cellcom, "SUB", variables[operand1].address));
        cellcom++;
        commandVector.push_back(commandSA(cellcom, "JNEG", cellcom + 2));
        cellcom++;
        commandVector.push_back(commandSA(cellcom, "JUMP", -1));
        jumpboy = commandVector.size() - 1;
        cellcom++;
    } else if (parsedifline[2] == "<") {
        commandVector.push_back(commandSA(cellcom, "LOAD", variables[operand1].address));
        cellcom++;
        commandVector.push_back(commandSA(cellcom, "SUB", variables[operand2].address));
        cellcom++;
        commandVector.push_back(commandSA(cellcom, "JNEG", cellcom + 2));
        cellcom++;
        commandVector.push_back(commandSA(cellcom, "JUMP", -1));
        jumpboy = commandVector.size() - 1;
        cellcom++;
    }
    cmatch letOneLine;
    cmatch letTwoLine;
    string command = parsedifline[4];
    if (command == "REM") {
        cout << "КОММЕНТАРИЙ В ИФЕ" << endl;
        exit(-1);
    } else if (command == "INPUT") {
        inputFun(parsedifline[5]);
    } else if (command == "PRINT") {
        printFun(parsedifline[5]);
    } else if (command == "GOTO") {
        gotoFun(parsedifline[5]);
    } else if (command == "LET") {
        letFun(parsedifline[5], letOneLine, letTwoLine);
    } else if (command == "END") {
        endFun(parsedifline[5]);
    }
    commandVector[jumpboy].operand = cellcom;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return -1;
    }
    cmatch parsedLine;
    const regex sb(R"(\w+\.sb)");
    regex sa(R"(\w+\.sa)");
    cmatch letOneLine;
    cmatch letTwoLine;
    if (!regex_match((string) argv[1], sb)) {
        cout << "НЕВЕРНЫЙ ВВОД" << endl;
    }
    if (!regex_match((string) argv[2], sa)) {
        cout << "НЕВЕРНЫЙ ВВОД" << endl;
    }
    ifstream sbFile(argv[1]);
    string line;
    int prev = -1;
    while (getline(sbFile, line)) {
        if (line == "")
            continue;
        if (!regex_match(line.c_str(), parsedLine, simplecheck)) {
            cout << "НЕВЕРНАЯ СТРОКА" << " " << line << endl;
            return -1;
        }
        if(prev >= stoi(parsedLine[1]) ){
            cout << "НЕВЕРНЫЕ НОМЕРА СТРОК" << endl;
            return -1;
        }
        prev = stoi(parsedLine[1]);
        string command = parsedLine[2];
        if (command == "REM") {
            continue;
        } else if (command == "INPUT") {
            commandStart[parsedLine[1]] = cellcom;
            inputFun(parsedLine[3]);
        } else if (command == "PRINT") {
            commandStart[parsedLine[1]] = cellcom;
            printFun(parsedLine[3]);
        } else if (command == "GOTO") {
            commandStart[parsedLine[1]] = cellcom;
            gotoFun(parsedLine[3]);
        } else if (command == "LET") {
            commandStart[parsedLine[1]] = cellcom;
            letFun(parsedLine[3], letOneLine, letTwoLine);
        } else if (command == "END") {
            commandStart[parsedLine[1]] = cellcom;
            endFun(parsedLine[3]);
            break;
        } else if (command == "IF") {
            commandStart[parsedLine[1]] = cellcom;
            ifFun(parsedLine[3]);
        }
    }
    if (cellcom > cellvar) {
        cout << "ПЕРЕИЗБЫТОК ПЕРЕМЕННЫХ" << endl;
        return -1;
    }

    for (auto &i : gotoQueue) {
        if (commandStart.find(to_string(commandVector[i].operand)) == commandStart.end()) { // Если не нашел
            cout << "ГОТУ ССЫЛАЕТСЯ НА СТРОКУ КОТОРОЙ НЕТ" << endl;
            return -1;
        } else {
            short addressOperand = commandStart.find(to_string(commandVector[i].operand))->second;
            commandVector[i].operand = addressOperand;
        }
    }
    ofstream saFile(argv[2]);
    for (auto &i: commandVector) {
        saFile << setfill('0') << setw(2) << i.cell << " " << i.command << " " << setfill('0') << setw(2) << i.operand
               << endl;
    }
    for (int i = variables.size() - 1; i >= 0; i--) {
        if (regex_match(variables[i].name, varcheck)) {
            saFile << setfill('0') << setw(2) << variables[i].address << " " << "=" << " " << setfill('0') << setw(4)
                   << "0" << endl;
        } else {
            if (stoi(variables[i].name) < 0) {
                saFile << setfill('0') << setw(2) << variables[i].address << " " << "=";
                saFile << " -" << setfill('0') << setw(4) << hex << uppercase << -stoi(variables[i].name) << endl;
                saFile << dec;
            } else {
                saFile << setfill('0') << setw(2) << variables[i].address << " " << "=";
                saFile << " " << setfill('0') << setw(4) << hex << uppercase << stoi(variables[i].name) << endl;
                saFile << dec;
            }
        }
    }
    return 0;
}
