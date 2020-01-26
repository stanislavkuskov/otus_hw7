// Наблюдатель (Observer)

#include <iostream>
#include <vector>

enum class Lang
{
    ru, en
};

class Observer {
public:
    virtual void update(Lang lang) = 0;
};

class Language {
    Lang lang{Lang::ru};
    std::vector<Observer *> subs;
public:
    void subscribe(Observer *obs) {
        subs.push_back(obs);
    }

    void set_language(Lang lang_) {
        lang = lang_;
        notify();
    }

    void notify() {
        for (auto s : subs) {
            s->update(lang);
        }
    }
};

class report_observer : public Observer {
public:
    report_observer(Language *lang) {
        lang->subscribe(this);
    }

    void update(Lang lang) override {
        std::cout << "switch report template to lang " << int(lang) << std::endl;
    }
};

class ui_observer : public Observer {
public:
    ui_observer(Language *lang) {
        lang->subscribe(this);
    }

    void update(Lang lang) override {
        std::cout << "refresh ui for lang " << int(lang) << std::endl;
    }
};

void print_cmds(const std::vector<std::string>& commands){
    for (const std::string& cmd : commands){
        std::cout << cmd << " ";
    }
    std::cout << std::endl;
};

int main(int, char *[]) {

    std::vector<std::string> commands;
    int N;
    std::cin >> N;

    size_t i = 0;
//    bool is_opened_block = false;
//    bool is_closed_block = true;
    int nesting = 0;

    while (true){
        ++i;
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "{"){
            ++nesting;
//            is_opened_block = true;
//            is_closed_block = false;
            if (nesting == 0){
                std::cout << nesting;
            }

        }
        if (cmd == "}"){
            if (nesting == 1){
                print_cmds(commands);
            }

            --nesting;
//            is_opened_block = false;
//            is_closed_block = true;
        }

        bool is_counted = nesting == 0;
        if (is_counted){
            commands.push_back(cmd);
            if (i == N){
                print_cmds(commands);
                i = 0;
                commands.clear();
            }
        }

        if (cmd == "<EOF>"){

            break;
        }

    }
    return 0;
    std::cout << N << std::endl;


    Language lang;

    report_observer rpt(&lang);
    ui_observer ui(&lang);

    lang.set_language(Lang::ru);

    lang.set_language(Lang::en);

    return 0;
}
