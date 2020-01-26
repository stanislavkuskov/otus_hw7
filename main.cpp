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
    int nesting = 0;



    while (true){
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "{"){
            if ((nesting == 0) and (!commands.empty())){
                print_cmds(commands);
                commands.clear();
                i = 0;
            }
            ++nesting;
        }

        else if (cmd == "}"){
            if (nesting == 1){
                print_cmds(commands);
                commands.clear();
                i = 0;
            }
            --nesting;
        }

        else if (cmd == "<EOF>"){
            break;
        }

        else {
            commands.push_back(cmd);
        }

        bool is_counted = nesting == 0;
        if (is_counted){
            if (i == N){
                print_cmds(commands);
                commands.clear();
                i = 0;
            }
            ++i;
        }
    }
    return 0;


    Language lang;

    report_observer rpt(&lang);
    ui_observer ui(&lang);

    lang.set_language(Lang::ru);

    lang.set_language(Lang::en);

    return 0;
}
