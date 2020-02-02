// Наблюдатель (Observer)

#include <ctime>
#include <iostream>
#include <fstream>
#include <ostream>
#include <utility>
#include <vector>

using cmd_arr = std::vector<std::string>;

///Obsrerver
class Observer {
public:
    virtual void update(cmd_arr commands, std::string filename) = 0;
};

class Writer {
    std::string filename;
    cmd_arr commands;
    std::vector<Observer *> subs;
public:
    void subscribe(Observer *obs) {
        subs.push_back(obs);
    }

    void run_writing(cmd_arr commands_, std::string filename_) {
        commands = std::move(commands_);
        filename = std::move(filename_);
        notify();
    }

    void notify() {
        for (auto s : subs) {
            s->update(commands, filename);
        }
    }
};

class CoutObserver : public Observer {
public:
    CoutObserver(Writer *writer) {
        writer->subscribe(this);
    }

    void update(cmd_arr commands, std::string filename) override {
        for (const auto& cmd : commands){
            std::cout << cmd << " ";
        }
        std::cout << std::endl;
    }
};

class FileObserver : public Observer {
public:
    FileObserver(Writer *writer) {
        writer->subscribe(this);
    }

    void update(cmd_arr commands, std::string filename) override {
        std::ofstream myfile;
        myfile.open(filename);
        for (const auto& cmd : commands){
            myfile << cmd << "\n";
        }
        myfile.close();

    }
};

int main(int, char *[]) {

    Writer writer;
    CoutObserver couter(&writer);
    FileObserver filer(&writer);

    cmd_arr commands;
    size_t N;
    std::cin >> N;
    //  some black magic from https://stackoverflow.com/questions/257091/how-do-i-flush-the-cin-buffer
    //  do not touch it
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    size_t i = 1;
    int nesting = 0;

    bool turn = false;

    std::string filename;
    while (true){
        std::string cmd;
        std::getline(std::cin, cmd);

        {
            if (commands.empty()){
                filename = "bulk" + std::to_string(std::time(nullptr)) + ".log";
            }
        }

        {
            if (cmd == "{"){
                if ((nesting == 0) and (!commands.empty())){
                    turn = true;
                }
                ++nesting;
            }

            else if (cmd == "}"){
                if (nesting == 1){
                    turn = true;
                }
                --nesting;
            }

            else if (cmd == "<EOF>"){
                break;
            }

            else {
                commands.push_back(cmd);
            }
        }
        {
            if (nesting == 0){
                if (i == N){
                    turn = true;
                }
                ++i;
            }

            if (turn){
                writer.run_writing(commands, filename);
                commands.clear();
                i = 1;
                turn = false;
            }
        }
    }
    return 0;
}
