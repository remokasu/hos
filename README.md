# 概要
![Example Image](./img.gif)

## ダウンロード
~~~ bash
git clone git@github.com:remokasu/hos.git
~~~

## libncurses5-dev, libncursesw5-dev のインストール
~~~ bash
sudo apt-get install libncurses5-dev libncursesw5-dev
~~~

## コンパイル
~~~ bash
cd hos
gcc -o hos hos.c -lncurses
~~~

## 実行
~~~ bash
./hos
~~~

## 使い方
パスワード入力でE.HOBAを入力

## 終了
Ctrl + C

## 免責事項
- このプログラムは冗談半分で作成されたものであり、完全に無害です。

- このプログラムを使用したことによるいかなる損害も責任を負いません。


## Dependencies

This project uses the `ncurses` library, which is an open-source, terminal handling library that provides an API for building text-based interfaces in a terminal-independent manner.

### ncurses License

`ncurses` is licensed under a modified X11 license, which is similar to the MIT License but specifically adapted for the `ncurses` project. This license allows for free use, modification, and redistribution of the library.

The full text of the `ncurses` license can be found in the `ncurses` distribution or on the [official ncurses repository](https://invisible-island.net/ncurses/announce.html) where it is maintained.

### Attribution

This project incorporates the `ncurses` library by Thomas E. Dickey and other contributors. The usage of `ncurses` within this project adheres to its licensing terms.

