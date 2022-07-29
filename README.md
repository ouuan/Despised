# Despised

[![Build Test](https://github.com/ouuan/Despised/workflows/Build%20Test/badge.svg)](https://github.com/ouuan/Despised/actions?query=workflow%3A%22Build+Test%22)
[![AUR version](https://img.shields.io/aur/version/noi-despised-git)](https://aur.archlinux.org/packages/noi-despised-git/)

乱写的一个背笔试的小程序，很多预想的功能都咕掉了..

主要特色可能是记录用时与正确率，着重测验不熟悉的题目？（然而导入历史记录被咕掉了）

（而且使用键盘输入，非常方便。）

（大）部分题目数据来自 https://duck.ac/beibishi ，每年会根据笔试题库更新题目。（如果发现题目有锅请开 [issue](https://github.com/ouuan/Despised/issues) 或 [PR](https://github.com/ouuan/Despised/pulls)）

## 使用说明

输入选项编号来回答问题，如 "2"，"3"，"41"，"123"。

正常情况下程序不会终止，可以输入 `q` 或者按 <kbd>Ctrl + C</kbd> 退出，退出时会将错过的题目输出至 stderr，如果需要导出错误题目的话可以将 stderr 重定向至文件（`./despised 2>wrong.txt`，就是重定向 stdout 的 `>` 前面加个 `2`）。（用 stderr 输出错误题目是因为 ~~它名字就叫标准错误流~~ 如果用 stderr 显示题目清屏有些问题。不用文件输出是因为自从我把 despised 安装到 `/usr/bin` 后我随手做笔试经常会在奇奇怪怪的地方多出来一个 `wrong.txt`。）

## F & Q

Q：打开后是乱码。

A：代码是使用 UTF-8 编码保存的，你可以尝试修改命令行的文字编码，或者用其它编码重新保存代码。

Q：为什么连续几道题都是同一道？

A：题目顺序是按照正确率、AC 次数和平均 AC 用时（加上一些随机成分）使用 `std::priority_queue` 决定的。正常使用时，只要不连续错一道题多次、一道题花太长时间，都是不会有太多的连续同一道题的。如果觉得参数不合适可以自行修改。事实上刚错过再考一遍也可以加深印象。

## License

`pdf` 文件夹下的笔试题库仅供大家方便而放在这，ouuan 不对其拥有版权。该项目的其它部分以 MIT License 发布。
