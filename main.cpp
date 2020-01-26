// Наблюдатель (Observer)

#include <iostream>
#include <vector>

using cmd_arr = std::vector<std::string>;

class Observer {
public:
    virtual void update(cmd_arr commands) = 0;
};

class Writer {
    cmd_arr commands;
    std::vector<Observer *> subs;
public:
    void subscribe(Observer *obs) {
        subs.push_back(obs);
    }

    void run_writing(cmd_arr cmds) {
        commands = cmds;
        notify();
    }

    void notify() {
        for (auto s : subs) {
            s->update(commands);
        }
    }
};

class CoutObserver : public Observer {
public:
    CoutObserver(Writer *writer) {
        writer->subscribe(this);
    }

    void update(cmd_arr commands) override {
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

    void update(cmd_arr commands) override {
// TODO запись в файл
        std::cout << "write in file " << std::endl;

    }
};

int main(int, char *[]) {

    Writer writer;
    CoutObserver couter(&writer);
    FileObserver filer(&writer);

    cmd_arr commands;
    int N;
    std::cin >> N;

    size_t i = 1;
    int nesting = 0;

    bool turn = false;

    while (true){
        std::string cmd;
        std::cin >> cmd;
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
//                print_cmds(commands);

                writer.run_writing(commands);
                commands.clear();
                i = 1;
                turn = false;
            }
        }

    }
    return 0;
}
