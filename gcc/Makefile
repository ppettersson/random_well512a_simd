.DEFAULT_GOAL := all

# --- Input variables ---
BUILD ?= release
ifeq ($(findstring $(BUILD),debug release),)
  $(error BUILD must be set to debug or release)
endif

# Build all executables by default.
APPS ?= well512a

# If the VERBOSE flag isn't set, then mute superfluous output.
ifeq ($(VERBOSE),)
  HUSH_COMPILE = @echo "Compiling $<";
  HUSH_LINK = @echo "Linking $@";
  HUSH_GENERATE = @echo "Generating $@";
  HUSH_CLEAN = @
endif

# --- Internal variables ---
ARCH := $(shell uname -p)
SRC_ROOT := $(abspath ..)
OUTPUT_DIR := $(SRC_ROOT)/gcc
INTERMEDIATE_DIR := $(SRC_ROOT)/gcc/obj
BUILD_DIR := $(INTERMEDIATE_DIR)/$(BUILD)

ARFLAGS = r
LDFLAGS =

# Always enable debug information.
CFLAGS += -g

# Flags to generate dependency information.
CFLAGS += -MD -MP -MT $@

# Predefined flags.
ifeq ($(BUILD), debug)
  CFLAGS += -D_DEBUG
endif

# Enable compiler optimizations for everything except debug.
# Note that a very aggresssive optimization level is used and it may not be
# valid for all standard compliant programs. Reduce this level on individual
# files or modules as needed.
ifneq ($(BUILD), debug)
  CFLAGS += -Ofast
endif

# Flag to turn on extended instruction sets for the compiler.
CFLAGS += -msse2

# Let C++ inherit all C flags.
CXXFLAGS = $(CFLAGS)

# --- Internal functions ---
app_exe = $(OUTPUT_DIR)/$(1)_$(ARCH)_$(BUILD)
objs_from_src = $(patsubst $(SRC_ROOT)/%, $(BUILD_DIR)/%.o, $(basename $(1)))
objs_from_src_in = $(call objs_from_src, $(shell find $(1) -name "*.cpp" -o -name "*.c"))
objs_from_src_in_no_recurse = $(call objs_from_src, $(shell find $(1) -maxdepth 1 -name "*.cpp" -o -name "*.c"))


# --- well512a application ---
ifneq ($(filter well512a,$(APPS)),)

WELL512A_EXE := $(call app_exe,well512a)
WELL512A_OBJS := $(call objs_from_src_in,$(SRC_ROOT))
EXES += $(WELL512A_EXE)
OBJS += $(WELL512A_OBJS)

$(WELL512A_EXE): $(WELL512A_OBJS) $(LIBS)

endif


# --- Build rules ---

# Dependencies.
DEPS = $(OBJS:.o=.d)
-include $(DEPS)

.PHONY: all clean cleanall help

all: $(EXES)

clean:
	@echo "Cleaning..."
	$(HUSH_CLEAN) $(RM) -r $(BUILD_DIR)

cleanall:
	@echo "Cleaning all..."
	$(HUSH_CLEAN) $(RM) -r $(INTERMEDIATE_DIR)

help:
	@echo "BUILD   = Build mode. One of:"
	@echo "          debug (default)"
	@echo "          release (optimizations)"
	@echo "APPS    = Applications to build. Defaults to all."
	@echo "VERBOSE = Full output from commands if set."

# It's important that libraries are specified last as Ubuntu uses "ld --as-needed" by default.
# Only the static libraries referenced by the object files will be linked into the executable.
# Beware that circular dependencies doesn't work with this flag.
$(EXES):
	@mkdir -p $(@D)
	$(HUSH_LINK) $(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.a:
	@mkdir -p $(@D)
	$(HUSH_GENERATE) $(AR) $(ARFLAGS) $@ $^
#	$(HUSH_GENERATE) $(AR) $(ARFLAGS) $@ $^ 2> /dev/null

$(BUILD_DIR)/%.o: $(SRC_ROOT)/%.c
	@mkdir -p $(@D)
	$(HUSH_COMPILE) $(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_ROOT)/%.cpp
	@mkdir -p $(@D)
	$(HUSH_COMPILE) $(CXX) -c $(CXXFLAGS) -o $@ $<
