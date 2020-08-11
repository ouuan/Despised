#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <fstream>
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
    double totalAcTime;
    int acCount, totalCount;

    Problem(const string &_statement, const vector<Option> &_options) : statement(_statement), options(_options), totalAcTime(0), acCount(0), totalCount(0)
    {}

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
        os << problem.statement << endl;
        for (int i = 0; i < (int)problem.options.size(); ++i)
            os << i + 1 << ") " << problem.options[i].text << endl;
        return os;
    }
};

vector<Problem> problems = {
    {"NOI 机试使用的操作系统是", {{"Linux", true}, {"Windows"}, {"Mac OS"}}},
    {"Linux 中为文件改名使用的命令是", {{"mv <旧文件名> <新文件名>", true}, {"mv <新文件名> <旧文件名>"}, {"rename <旧文件名> <新文件名>"}, {"rename <新文件名> <旧文件名>"}}},
    {"在 Linux 中返回上一级目录使用的命令是", {{"cd ..", true}, {"cd ."}, {"cd /"}, {"cd ~"}}},
    {"在 Linux 中删除当前目录下的 test 目录的命令是", {{"rm -r test", true}, {"rm test"}, {"rm -f test"}, {"rm /test"}}},
    {"当前目录下有一个编译好的可执行文件 a.out，执行它使用的命令是", {{"./a.out", true}, {"/a.out"}, {"a.out"}, {"../a.out"}}},
    {"使用高级语言编写的程序称之为", {{"源程序", true}, {"目标文件"}, {"高级程序"}, {"高级代码"}}},
    {"在 NOI Linux 系统中可以用来调试程序的程序是", {{"gdb", true}, {"gcc"}, {"vim"}, {"g++"}}},
    {"在 Linux 系统中，文件夹中的文件可以与该文件夹同名吗", {{"可以", true}, {"不可以"}}},
    {"Linux 系统中杀死名为 test 的后台进程的命令是", {{"killall test", true}, {"kill test"}, {"rm test"}, {"close test"}}},
    {"Linux 系统中可以查看隐藏文件的命令是", {{"ls -a", true}, {"ls -l"}, {"ls -h"}, {"ls -R"}}},
    {"Linux 系统中编译 C++ 程序的编译器是", {{"g++", true}, {"gcc"}, {"gdb"}, {"fpc"}}},
    {"Linux 系统中，将当前目录下的文件名打印到 tmp 文件中的命令是", {{"ls > tmp", true}, {"ls < tmp"}, {"ls tmp"}, {"ls -o tmp"}}},
    {"Linux 系统中，测量当前目录下程序 test 运行时间的命令是", {{"time ./test", true}, {"time test"}, {"time ../test"}, {"time test.exe"}}},
    {"vim 编辑器中，强制退出不保存修改应当输入", {{":q!", true}, {":wq"}, {":w!"}, {"q"}}},
    {"vim 编辑器中，强制退出并保存修改可输入以下三种命令之一", {{":wq", true}, {"ZZ", true}, {":x", true}, {":w!"}}},
    {"vim 编辑器中，定位到文件中第 12 行应当输入", {{":12", true}, {"12"}, {":l12"}, {"/12"}}},
    {"vim 编辑器中，在文件中查找字符串“12”应当输入", {{"/12", true}, {":12"}, {":l12"}, {"12"}}},
    {"使用 g++ 编译 C++ 程序时，生成调试信息的命令行选项是", {{"-g", true}, {"-o"}, {"-c"}, {"-d"}}},
    {"使用 g++ 编译 C++ 程序时，生成所有警告信息的命令行选项是", {{"-Wall", true}, {"-Werror"}, {"-g"}, {"-o"}}},
    {"使用 g++ 编译 C++ 程序时，只编译生成目标文件的命令行选项是", {{"-c", true}, {"-o"}, {"-g"}, {"-d"}}},
    {"使用 g++ 编译 C++ 程序时，指定输出文件名的命令行选项是", {{"-o", true}, {"-c"}, {"-g"}, {"-d"}}},
    {"如果 C++ 程序中使用了 math.h 中的函数，在用 g++ 编译时需要加入选项", {{"-lm", true}, {"-lmath"}, {"-m"}, {"-math"}}},
    {"Linux 系统中具有最高权限的用户是", {{"root", true}, {"sudo"}, {"admin"}, {"friend"}}},
    {"在 Linux 的各个虚拟控制台间切换的快捷键是", {{"Ctrl+Alt+Fn", true}, {"Ctrl+Alt+Del"}, {"Ctrl+Alt+PageDown"}, {"Ctrl+Alt+PageUp"}}},
    {"在 NOI Linux 中，从字符控制台切换回桌面环境使用的快捷键是", {{"Ctrl+Alt+F7", true}, {"Ctrl+Alt+F1"}, {"Ctrl+Alt+Del"}, {"Ctrl+Alt+F12"}}},
    {"在 NOI Linux 中默认使用的 Shell 是", {{"bash", true}, {"sh"}, {"zsh"}, {"shell"}}},
    {"在 Linux 中查看当前系统中的进程使用的命令是", {{"ps", true}, {"ls"}, {"cd"}, {"time"}}},
    {"在 Linux 中查看进程的 CPU 利用率使用的命令是", {{"ps", true}, {"ls"}, {"cd"}, {"time"}}},
    {"在终端中运行自己的程序，如果自己的程序进入死循环，应当如何终止", {{"Ctrl-C", true}, {"Ctrl-A"}, {"Ctrl-X"}, {"Ctrl+Alt+Del"}}},
    {"可执行文件 a.out 从标准输入读取数据。现有一组输入数据保存在 1.in 中，使用这个测试数据文件测试自己的程序的命令是", {{"./a.out < 1.in", true}, {"./a.out > 1.in"}, {"./a.out 1.in"}, {"./a.out -o 1.in"}}},
    {"可执行文件 prog_1 向标准输出输出运行结果。将输出结果保存到 1.out 文件中使用的命令是", {{"./prog_1 > 1.out", true}, {"./prog_1 < 1.out"}, {"./prog_1 1.out"}, {"./prog_1 -o 1.out"}}},
    {"使用主机“重启”键强行重新启动计算机，可能会对系统造成的后果是", {{"文件系统损坏", true}, {"硬盘损坏"}, {"CPU 过热"}, {"无法开机"}}},
    {"在 Linux 系统中，用于查看文件的大小的命令是", {{"ls -l", true}, {"ls -a"}, {"ls -h"}, {"ls -R"}}},
    {"当前目录中有如下文件：\n-rw-r--r-- 1 user None 8.7K Jul 2 16:35 foobar\n-rw-r--r-- 1 user None 93 Jul 2 16:35 foobar.c++\n-rwx------ 1 user None 144 Jul 2 16:35 foobar.sh\n其中，可以执行的文件是", {{"foobar.sh", true}, {"foobar.c++"}, {"foobar"}, {"都不可执行"}}},
    {"评测系统中对程序源文件大小的限制是", {{"小于 100KB", true}, {"小于 50KB"}, {"小于 64KB"}, {"小于 128KB"}}},
    {"如无另行说明，评测系统中对程序使用内存的限制是", {{"以硬件资源为限", true}, {"2GB"}, {"512MB"}, {"256MB"}}},
    {"Linux 下的换行字符为", {{"\\n", true}, {"\\r"}, {"\\r\\n"}, {"enter"}}},
    {"终止一个失去响应的进程（$pid 代表进程号）的命令是", {{"kill $pid", true}, {"killall $pid"}, {"rm $pid"}, {"stop $pid"}}},
    {"Linux 中是否区分文件和目录名称的大小写", {{"是", true}, {"否"}}},
    {"选手在 NOI 机试过程中是否禁止使用网络", {{"是", true}, {"否"}}},
    {"源代码文件 my.cpp 创建一个备份 mycpp.bak 时，使用的命令是", {{"cp my.cpp mycpp.bak", true}, {"cp mycpp.bak my.cpp"}, {"mv my.cpp mycpp.bak"}, {"mv mycpp.bak my.cpp"}}},
    {"调试程序的方法有", {{"单步调试", true}, {"使用 print 类语句打印中间结果", true}, {"读源代码", true}, {"使用小黄鸭"}}},
    {"在考试过程中，如果出现系统死机或者崩溃现象，选手应当采取的措施是", {{"举手示意监考人员处理", true}, {"自行重启机器"}, {"按下 Ctrl+Alt+Del"}, {"按 Reset 键"}}},
    {"提交的答案程序中如果包含 NOI 考试明确禁止使用的代码，后果是", {{"本题成绩以 0 分计算", true}, {"本场比赛成绩以 0 分计算"}, {"取消比赛资格"}, {"取消获奖资格"}}},
    {"对评测结果有疑义，需要申请复评，则", {{"提出书面申请", true}, {"经领队、有关工作人员、科学委员会主席签字确认", true}, {"提交至评测人员", true}, {"经教练签字"}}},
    {"复评成绩较原始成绩有变化，则", {{"以复评成绩为准", true}, {"以原始成绩为准"}, {"取两者最高分"}, {"由评测人员决定"}}},
    {"NOI 考试对 C++ 语言标准模板库的使用有限制吗？", {{"没有", true}, {"有"}}},
    {"NOI 考试对 C++ 语言使用有限制吗？", {{"有", true}, {"没有"}}},
    {"名为 FILE 的文件和名为 File 的文件在 Linux 系统中被认为是", {{"不同的文件", true}, {"相同的文件"}}},
    {"目录 DIRECT 和目录 Direct 在 Linux 系统中被认为是", {{"不同的目录", true}, {"相同的目录"}}},
    {"在 NOI 正式考试中如何登录自己的比赛用机", {{"使用考前工作人员下发的账户及密码", true}, {"使用 root 账户"}, {"使用 friend 账户"}, {"使用试机时下发的账户及密码"}}},
    {"如果考试分多日进行，那么选手的考试账户和口令", {{"由工作人员在每场考试开始前下发", true}, {"试机之前统一下发"}, {"每次考试一样"}, {"在考试前放在选手的桌上"}}},
    {"考试结束后，应如何处理密码条", {{"保存好密码条，用于复测", true}, {"可以丢弃密码条"}, {"将密码告诉别人"}, {"随便怎么处理"}}},
    {"选手答案文件保存的目录是", {{"选手目录下和考题名称相同的目录", true}, {"选手目录"}, {"/home"}, {"根目录"}}},
    {"选手答案的文件名要求是", {{"和试卷的题目摘要中所示文件名一致", true}, {"answer"}, {"任何文件名都可以"}, {"code"}}},
    {"选手答案的文件名大小写错误，成绩会怎样", {{"0分", true}, {"100分"}, {"不变"}}},
    {"选手提交的源代码文件名是否有特殊要求", {{"源程序文件名由试题名称缩写加后缀构成", true}, {"试题名称缩写及后缀一律使用小写", true}, {"试题名称缩写及后缀一律使用大写"}, {"试题名称首字母大写，后缀小写"}}},
    {"在 NOI 考试中，C++ 源文件的扩展名规定为", {{"cpp", true}, {"c"}, {"pas"}, {"txt"}}},
    {"发现鼠标或其他硬件设备有问题，选手可以", {{"请工作人员更换", true}, {"重启电脑"}, {"重新插入有问题的硬件设备"}, {"关机"}}},
    {"对试题理解有问题，选手可以", {{"举手求助", true}, {"和边上选手讨论"}, {"自行处理"}, {"做其他题目"}}},
    {"考试结束后选手需要", {{"迅速离开", true}, {"在原地等待评测"}, {"在考场内讨论题目"}, {"继续答题"}}},
    {"复评结束后是否还能提交复评申请", {{"不能", true}, {"能"}}},
    {"测试点时间限制的含义是指", {{"题目允许程序运行所占用的用户时间总和的上限值", true}, {"题目允许程序运行所占用的实际时间总和的上限值"}, {"题目允许程序编译所占用的用户时间总和的上限值"}, {"题目允许程序编译所占用的实际时间总和的上限值"}}},
    {"什么情况下选手可以申请延长考试时间", {{"机器出现故障，并由工作人员确认和记录", true}, {"对试题理解有问题"}, {"忘记保存源程序"}, {"水喝完了"}}},
    {"考试中选手自行重新启动机器，能否获得加时？", {{"不能", true}, {"能"}}},
    {"草稿纸用完了，如何处理", {{"举手向监考人员求助", true}, {"向边上选手要一张"}, {"使用自己带的草稿纸"}, {"在桌上打草稿"}}},
    {"水喝完了，如何处理", {{"举手向监考人员再要一瓶", true}, {"向边上选手要一瓶"}, {"喝自己带的水"}, {"不能喝更多的水"}}},
    {"考试太简单，能提前离开吗？", {{"能", true}, {"不能"}}},
    {"离开考场后，发现还有个问题没改，能回去再改吗？", {{"不能", true}, {"能"}}},
    {"考试中机器突然没响应了，如何处理", {{"举手向监考人员求助", true}, {"重启机器"}, {"关机"}, {"等待机器响应"}}},
    {"考试中发现登录名和密码的单子丢了，如何处理", {{"向工作人员求助", true}, {"离开考场"}, {"使用 root 账户登录"}, {"无法继续考试"}}},
    {"复评的时候忘记登录名和密码了，如何处理", {{"向工作人员求助", true}, {"使用 friend 账户登录"}, {"使用 root 账户登录"}, {"无法进行复评"}}},
    {"在监考人员宣布 NOI 机试开始之前，是否允许选手登录系统和翻阅试卷", {{"否", true}, {"是"}}},
    {"在 NOI 上机考试中，是否允许选手私自重新启动计算机？", {{"否", true}, {"是"}, {"死机了就允许"}}},
    {"在 NOI 系列考试中，如果由于文件名不正确导致被判 0 分，提出复评请求，会被接受吗", {{"不会", true}, {"会"}}},
    {"在 NOI 系列考试中，如果由于文件目录名不正确导致被判 0 分，提出复评请求，会被接受吗", {{"不会", true}, {"会"}}},
    {"在 NOI 系列考试中，如果由于文件保存路径不正确导致被判 0 分，提出复评请求，会被接受吗", {{"不会", true}, {"会"}}},
    {"选手可以不使用集成开发环境（IDE）编辑程序源代码吗", {{"可以", true}, {"不可以"}}},
    {"选手回答填空题，提交的答案中可以包含引号吗", {{"不可以", true}, {"可以"}}},
    {"选手程序在某测试点上的运行时间仅比时限多 0.005 秒，算不算超时", {{"算", true}, {"不算"}}},
    {"在 NOI 上机考试中，允许选手使用的编程语言包括", {{"C"}, {"C++", true}, {"Pascal"}, {"Python"}}},
    {"NOI 比赛的题目类型有", {{"非交互式程序题", true}, {"交互式程序题", true}, {"答案提交题", true}, {"填空题"}}},
    {"选手比赛中提交的有效文件类型有", {{"答案文件", true}, {"源程序", true}, {"输出文件"}, {"输入文件"}}},
    {"选手提交的程序不得进行的操作包括", {{"试图访问网络", true}, {"使用 fork 或其它线程/进程生成函数", true}, {"打开或创建题目规定的输入/输出文件之外的其它文件", true}, {"运行其它程序", true}}},
    {"以修改过的程序或答案为依据的申诉是否会被受理", {{"否", true}, {"是"}}},
    {"没有自行复测结果（包括结果正确性、所用时间、所用空间等）支持的申诉是否会被受理", {{"否", true}, {"是"}}},
    {"超过申诉时间的申诉是否会被受理", {{"否", true}, {"是"}}},
    {"对评测结果中的超时有异议，且复测结果的运行时间与题目时间限制之差小于题目时间限制 5％ 的申诉是否会被受理", {{"否", true}, {"是"}, {"题目卡常就会"}}},
    {"遇到下列哪些情况可以向工作人员申请加时补偿", {{"计算机硬件故障", true}, {"操作系统死机", true}, {"题目理解有问题"}, {"上厕所"}}},
    {"考试时若遇到计算机硬件故障或操作系统死机，应如何处理", {{"举手向工作人员求助", true}, {"关机"}, {"重启"}, {"向边上的选手求助"}}},
    {"选手进入考场可以携带的物品是", {{"笔", true}, {"手表", true}, {"手机"}, {"纸"}}},
    {"选手进入考场不可以携带的物品是", {{"纸", true}, {"U盘", true}, {"手机", true}, {"手表"}}},
    {"竞赛组织者将在竞赛场地为选手提供的物品是", {{"草稿纸", true}, {"饮用水", true}, {"食品", true}, {"笔"}}},
    {"一个完整的计算机系统应包括", {{"硬件系统", true}, {"软件系统", true}, {"操作系统"}, {"显示器"}}},
    {"目前微型计算机中采用的逻辑组件是", {{"大规模集成电路", true}, {"超大规模集成电路", true}, {"电子管"}, {"晶体管"}}},
    {"软件与程序的区别是", {{"软件是程序以及开发、使用和维护所需要的所有文档的总称", true}, {"程序是软件的一部分", true}, {"软件就是程序"}, {"程序不是软件的一部分"}}},
    {"IT 表示", {{"信息技术", true}, {"信息学"}, {"信息学竞赛"}, {"全国信息学竞赛"}}},
    {"计算机中央处理器简称为", {{"CPU", true}, {"RAM"}, {"ALU"}, {"HDD"}}},
    {"计算机内存储器的作用是用来存放", {{"当前 CPU 正在使用的程序", true}, {"当前 CPU 正在使用的数据", true}, {"计算机硬盘中的程序"}, {"计算机硬盘中的数据"}}},
    {"用来全面管理计算机硬件和软件资源的软件叫", {{"操作系统", true}, {"BIOS"}, {"Linux"}, {"固件"}}},
    {"LAN 是指", {{"局域网", true}, {"广域网"}, {"城域网"}, {"无线网络"}}},
    {"在微机中，bit 的中文含义是", {{"二进制位", true}, {"二进制"}, {"字节"}, {"字"}}},
    {"计算机所能辨认的最小信息单位是", {{"位", true}, {"字节"}, {"int"}, {"字"}}},
    {"ASCII 的含义是", {{"美国信息交换标准代码", true}, {"中国信息交换标准代码"}}},
    {"在计算机术语中经常用 RAM 表示", {{"随机存取存储器", true}, {"只读存储器"}, {"硬盘"}, {"顺序存取存储器"}}},
    {"RAM 存储器在断电后，其中的数据", {{"会变化", true}, {"不会变化"}}},
    {"ROM 存储器在断电后，其中的数据", {{"不会变化", true}, {"会变化"}}},
    {"现代计算机所应用的存储程序原理是由谁提出的", {{"冯·诺依曼", true}, {"阿兰·图灵"}, {"Dijkstra"}, {"Tarjan"}}},
    {"计算机内所有的信息都是以什么形式表示的", {{"二进制数码", true}, {"十进制数码"}, {"十六进制数码"}, {"八进制数码"}}},
    {"计算机直接识别和执行的语言是", {{"机器语言", true}, {"汇编语言"}, {"二进制"}, {"高级语言"}}},
    {"Linux 是一个怎样的操作系统，从而可以免费获得其源码", {{"开源", true}, {"闭源"}, {"古老"}, {"未编译"}}},
    {"NOI 的中文意思是", {{"全国信息学奥林匹克竞赛", true}, {"全国信息学奥林匹克联赛"}, {"国际信息学奥林匹克竞赛"}, {"国际信息学奥林匹克联赛"}}},
    {"字长为 32bit 的计算机,表示它能作为一个整体进行传送的数据长度可为多少个字节", {{"4", true}, {"1"}, {"8"}, {"32"}}},
    {"一个字节由相邻的多少个二进制位组成", {{"8", true}, {"1"}, {"4"}, {"16"}}},
    {"二进制数“10”化为十进制数是", {{"2", true}, {"1"}, {"10"}, {"0"}}},
    {"与十六进制数 0xAB 等值的二进数是", {{"10101011", true}, {"10101111"}, {"10000000"}, {"10100011"}}},
    {"Linux 中查看当前路径使用的命令是", {{"pwd", true}, {"ls"}, {"cat"}, {"ps"}}},
    {"在 Linux 下建立目录使用的命令是", {{"mkdir", true}, {"ls"}, {"rmdir"}, {"cat"}}},
    {"NOI 比赛中提供的 C++ IDE 环境有", {{"GUIDE", true}, {"Lazarus"}, {"Anjuta", true}, {"vim"}}},
    {"在 NOI 比赛中，编辑源代码除了可以用 GUIDE、Lazarus、Anjuta 等集成开发环境（IDE）之外,还可以使用", {{"vi", true}, {"gedit", true}, {"notepad"}, {"cat"}}},
    {"NOI 比赛每场上机考试的比赛时间是", {{"5 小时", true}, {"3 小时"}, {"10 小时"}, {"4 小时"}}},
    {"首届 NOI 是在哪一年举办的", {{"1984", true}, {"2000"}, {"1985"}, {"2017"}}},
    {"NOI2020 是第几届 NOI？", {{"37", true}, {"31"}, {"32"}, {"38"}}},
    {"IOI2020 是第几届 IOI？", {{"32", true}, {"33"}, {"36"}, {"37"}}},
    {"第 12 届 IOI 是哪一年在北京举办的", {{"2000", true}, {"1984"}, {"1985"}, {"2017"}}}};

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
    ofstream fout("wrong.txt");
    for (const auto &problem : problems)
    {
        if (problem.acCount == problem.totalCount)
            continue;
        fout << "AC Rate: " << problem.acCount << " / " << problem.totalCount << endl;
        fout << problem << endl;
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
