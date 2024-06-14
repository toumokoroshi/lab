

PROG := TARGET_NAME
SRCS := 
OBJS := $(SRCS:%.cpp=%.o)
DEPS := $(SRCS:%.cpp=%.d) # %マクロを用いて置換 SRC配列を元に"<ファイル名>.d"の配列を作る。 依存ファイル用。
でも、gccのオプションを活用するとなんと可能です！本人に聞けって感じですね。 実は、gccには依存ファイルをmake形式で書き出すという神みたいなオプションがあるんです。 main.cにmain.hとCONST.hというincludeを書いた状態で次のコマンドを発行します。

g++ -MMD -MP -c main.cpp


# (1)コンパイラ
CC  = gcc
# (2)コンパイルオプション
CFLAGS = -O2 -Wall -lm
# (3)実行ファイル名
PROG = exfile.out
# (4)コンパイル対象のソースコード
SRCS    = hoge1.cpp
SRCS    += hoge2.cpp
SRCS    += hoge3.cpp

# (9)ターゲットファイル生成
$(PROG): $(OBJS)
    $(CC) -o $@ 

# (10)オブジェクトファイル生成
$(OBJS): $(SRCS)
    $(CC) $(CFLAGS) -c $(SRCS)

# (11)"make all"で make cleanとmakeを同時に実施。
all: clean $(OBJS) $(TARGET)
# (12).oファイル、実行ファイル、.dファイルを削除
clean:
    -rm -f $(OBJS) $(TARGET) $(DEPS)

-include $(DEPS) # include "ファイル名" でそのファイルの内容をここにコピペしたのと同じ効果を得られる

