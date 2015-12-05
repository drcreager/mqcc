CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=CAny.cpp CBuffer.cpp CCollection.cpp CCursor.cpp CFile.cpp CFileCatalog.cpp CFileList.cpp CItem.cpp CString.cpp CStringList.cpp CSysCmd.cpp CWin32.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=common.so

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ 

