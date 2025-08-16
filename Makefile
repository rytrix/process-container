OUT_DIR = out
CC = gcc
DEFINES = -DDEBUG
OPTIMIZE = -O2
OUT_FILE = process-container
INSTALL_DIR = ~/.local/bin

$(OUT_DIR)/$(OUT_FILE): $(OUT_DIR) $(OUT_DIR)/main.o
	$(CC) $(OUT_DIR)/main.o -o $(OUT_DIR)/$(OUT_FILE) $(OPTIMIZE)

$(OUT_DIR)/main.o: src/main.c
	$(CC) src/main.c -o $(OUT_DIR)/main.o -c $(DEFINES) $(OPTIMIZE)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -r $(OUT_DIR)

install: $(OUT_DIR)/$(OUT_FILE)
	install -m 755 $(OUT_DIR)/$(OUT_FILE) $(INSTALL_DIR)

uninstall:
	-rm $(INSTALL_DIR)/$(OUT_FILE)

