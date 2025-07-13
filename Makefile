CC = gcc
CFLAGS =
TARGET = main
SOURCES = main.c Token.c Set.c ArvoreAVL.c TabelaHash.c Post.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)