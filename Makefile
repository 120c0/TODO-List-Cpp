include config/utils.mk
include config/lib/lib.mk
include config/main.mk

SRC_FILES = $(wildcard $(SRC_DIR)/*.$(EXTENSION_FILE)) $(wildcard $(SRC_DIR)/*/*.$(EXTENSION_FILE))
OBJECTS = $(SRC_FILES:%.$(EXTENSION_FILE)=%.o)
INCLUDES = -I$(INCLUDE_DIR) `pkg-config ncurses --cflags`
LIBS = -Llib `pkg-config ncurses --libs`
BUILD_TYPE = debug
TARGET_VERSION = 0.0.1
BUILD_FLAGS = -Wall -Werror -Wextra $(INCLUDES)

ifeq ($(BUILD_TYPE),release)
	BUILD_FLAGS += -O2 -DNDEBUG
else
	BUILD_FLAGS += -g -O0
endif

all: $(call main) $(OBJECTS) $(TARGET)
	
%.o: %.$(EXTENSION_FILE)
	@$(COMPILER) -c -o $@ $< $(BUILD_FLAGS)
	$(info [+] Compiling[$(notdir $@)] $(notdir $<)...)
$(TARGET): $(OBJECTS)
	@$(COMPILER) -o $@ $^ $(LIBS)
	$(info [+] Linking executable...)

clean:
	$(info Cleaning...)
	@$(RM) $(OBJECTS) $(TARGET)
	$(info Done!)
