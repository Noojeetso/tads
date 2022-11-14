CC := gcc -std=c99 -I./inc/
CFLAGS := -Wall -Werror -pedantic

OUT_DIR := ./out

SRC_DIR := ./src
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
OBJS := $(filter-out $(OUT_DIR)/main.o,$(OBJS))

.PHONY : clean debug release gcov unit func create_out_dir add_libs

app.exe : create_out_dir $(OBJS) $(OUT_DIR)/main.o
	$(CC) $(CFLAGS) $(OBJS) $(OUT_DIR)/main.o -o $@

func : gcov
	./run_func_tests.sh

$(OUT_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OUT_DIR)/%.o : $(UNIT_TESTS_DIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

create_out_dir:
	mkdir -p $(OUT_DIR)

gcov : CFLAGS += -O0 -g --coverage -dumpbase '' -DTEST
gcov : app.exe

debug : CFLAGS += -O0 -g -DDEBUG
debug : app.exe

release : app.exe

clean :
	rm -rf ./*.exe $(OUT_DIR)
