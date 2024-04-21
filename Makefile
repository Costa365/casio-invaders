# Define compiler
CXX=g++

# Define flags
CXXFLAGS=-lpthread -lncurses

# Define the output directories
OUTDIR=bin
OBJDIR=obj

# Define the target executable
TARGET=$(OUTDIR)/invaders

# Collect all cpp files in the directory
SRCS=$(wildcard *.cpp)

# Define object files based on source files
OBJS=$(SRCS:%.cpp=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(OUTDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Rule to compile all cpp files into object files
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)
	@rm -rf $(OBJDIR)

# Phony targets
.PHONY: all clean
