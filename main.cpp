#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <iostream>
#include <queue>
#include <random>
#include <set>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Option
{
    string text;
    bool correct;

    Option(const string &_text, bool _correct = false) : text(_text), correct(_correct)
    {}
};

struct Problem
{
    string statement;
    vector<Option> options;
    bool multianswer;
    double totalAcTime;
    int acCount, totalCount;

    Problem(const string &_statement, const vector<Option> &_options) : statement(_statement), options(_options), multianswer(false), totalAcTime(0), acCount(0), totalCount(0)
    {
        int answerCnt = 0;
        for (Option option : options)
        {
            answerCnt += option.correct;
        }
        if (answerCnt > 1)
        {
            multianswer = true;     
        }
    }

    double weight() const
    {
        return 1.0 * rng() / rng.max() - acCount - 10.0 * acCount / totalCount + (acCount ? totalAcTime / acCount : 10);
    }

    void shuffle()
    {
        std::shuffle(options.begin(), options.end(), rng);
    }

    bool check(const string &str) const
    {
        set<int> user, correct;

        for (int i = 0; i < (int)options.size(); ++i)
        {
            if (options[i].correct)
            {
                correct.insert(i);
            }
        }

        for (auto c : str)
            user.insert(c - '1');

        return correct == user;
    }

    string answer() const
    {
        string res;

        for (int i = 0; i < (int)options.size(); ++i)
        {
            if (options[i].correct)
            {
                res.push_back(i + '1');
            }
        }

        return res;
    }

    friend ostream &operator<<(ostream &os, const Problem &problem)
    {
        if (problem.multianswer) {
            os << "\033[34m[多选题]\033[0m ";
        }
        os << problem.statement << endl;
        for (int i = 0; i < (int)problem.options.size(); ++i)
            os << i + 1 << ") " << problem.options[i].text << endl;
        return os;
    }
};

#include "problem.conf"

double totalAcTime;
int acCount, totalCount, combo, distinctAc;

priority_queue<pair<double, int>> q;

void pause()
{
#ifdef _WIN32
    system("pause");
#else
    system("read -n1");
#endif
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ac(int id, double t)
{
    if (!problems[id].acCount)
        ++distinctAc;
    ++problems[id].acCount;
    ++problems[id].totalCount;
    ++acCount;
    ++totalCount;
    ++combo;
    totalAcTime += t;
    problems[id].totalAcTime += t;
    printf("\033[44;42;1mAccepted\033[0m in %.2lf seconds.\n\n", t);
    pause();
}

void wa(int id)
{
    ++problems[id].totalCount;
    ++totalCount;
    combo = 0;
    printf("\033[37;41;1mWrong Answer\033[0m, expected %s\n\n", problems[id].answer().c_str());
    pause();
}

void quit()
{
    for (const auto &problem : problems)
    {
        if (problem.acCount == problem.totalCount)
            continue;
        cerr << "AC Rate: " << problem.acCount << " / " << problem.totalCount << endl;
        cerr << problem << endl;
    }
    exit(0);
}

void signalHandler(int)
{
    quit();
}

void ask(int id)
{
    clearScreen();
    printf("distinct AC: %d / %d\tAC Rate: %d / %d\tCombo: %d\tAverageTime: %.2lf\n\n", distinctAc, (int)problems.size(), acCount, totalCount, combo, totalAcTime / acCount);
    problems[id].shuffle();
    cout << problems[id] << endl;

    auto startTime = chrono::steady_clock::now();
    string str;
    cin >> str;
    if (cin.fail() || str == "q")
        quit();
    auto endTime = chrono::steady_clock::now();

    if (problems[id].check(str))
        ac(id, 1.0 * chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count() / 1000);
    else
        wa(id);
}

int main()
{
    signal(SIGINT, signalHandler);

#ifdef _WIN32
    system("chcp 65001");
#endif

    clearScreen();
    puts("答题时请输入正确选项的编号，可能多选，编号间不带空格，顺序随意，输入 q 退出\n");
    puts("https://github.com/ouuan/Despised\n");
    puts("按任意键继续...");
    pause();

    for (int i = 0; i < (int)problems.size(); ++i)
        q.emplace(1.0 * rng() / rng.max(), i);

    while (true)
    {
        int u = q.top().second;
        q.pop();
        ask(u);
        q.emplace(problems[u].weight(), u);
    }

    return 0;
}
