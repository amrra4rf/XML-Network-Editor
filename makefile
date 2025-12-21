CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = SocialNetworkGraph.exe

SRC = \
	SRC/CLI.cpp \
	SRC/ClI_inputParser.cpp \
	SRC/compress.cpp \
	SRC/convert.cpp \
	SRC/decompress.cpp \
	SRC/Format.cpp \
	SRC/visualize.cpp \
	SRC/vaild.cpp \
	SRC/XML_Minifying.cpp \
	Parse/Reader.cpp \
	Graph/Graphs.cpp \
	Classes/User.cpp \
	Classes/Posts.cpp

INCLUDES = \
	-IHeader \
	-IParse \
	-IGraph \
	-IClasses

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(INCLUDES) -o $(TARGET)

clean:
	del /f $(TARGET)
