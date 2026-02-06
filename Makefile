CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -fPIC -I./include
LDFLAGS = -shared -Wl,--allow-shlib-undefined

SRC_DIR = .
INC_DIR = include
BUILD_DIR = build

TARGET = build/ExamModule.so
SOURCES = ExamModule.cpp
OBJECTS = $(BUILD_DIR)/ExamModule.o

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[1;33m
NC = \033[0m

.PHONY: all
all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link shared library
$(TARGET): $(OBJECTS)
	@echo "$(YELLOW)Linking $@...$(NC)"
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@
	@echo "$(GREEN)Build successful! Output: $(TARGET)$(NC)"

# Clean build artifacts
.PHONY: clean
clean:
	@echo "$(YELLOW)Cleaning build artifacts...$(NC)"
	@rm -rf $(BUILD_DIR) $(TARGET)
	@echo "$(GREEN)Clean complete!$(NC)"

# Rebuild from scratch
.PHONY: rebuild
rebuild: clean all

# Show help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all      - Build the module (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  rebuild  - Clean and build"
	@echo "  help     - Show this help message"

.PHONY: debug
debug: CXXFLAGS += -g -O0 -DDEBUG
debug: clean all
	@echo "$(GREEN)Debug build complete!$(NC)"

.PHONY: release
release: CXXFLAGS += -O3 -DNDEBUG
release: clean all
	@echo "$(GREEN)Release build complete!$(NC)"
