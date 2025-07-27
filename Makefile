include conf.mk

# Compiler and flags
CC 		 ?= cc
AR     ?= ar

CFLAGS ?= -Wall -Wextra -O2 -std=c99 -fPIC -I./include -march=native

LDFLAGS_SO = -shared
LDFLAGS_EXE =

ifeq ($(UT_VERBOSE), 0)
	CFLAGS += -D__UT_QUIET
endif

CFLAGS_SO     = $(CFLAGS)
CFLAGS_EXE    = $(CFLAGS)

SRCDIRS_CORE   = detail queue wunit metadata
SRCDIRS_COMMON = $(SRCDIRS_CORE) pool arena
SRCDIRS_EXE    = $(SRCDIRS_COMMON) unit etc

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

BUILD_DIR = build
OBJDIR = $(BUILD_DIR)/obj
BINDIR = $(BUILD_DIR)/bin

SRCS_SO  := $(call rwildcard,$(SRCDIRS_COMMON)/,*.c)
SRCS_EXE := $(call rwildcard,$(SRCDIRS_EXE)/,*.c)

SRCS_NO_MAIN := $(filter-out %/main.c,$(SRCS_SO))
MAIN_SRC     := $(filter %/main.c,$(SRCS_EXE))

OBJS_SO  := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS_NO_MAIN))
OBJS_EXE := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS_EXE))

LIBNAME    		= muda
LIBTARGET_SO  = $(BUILD_DIR)/lib$(LIBNAME).so
LIBTARGET_A   = $(BUILD_DIR)/lib$(LIBNAME).a

EXETARGET  = $(BUILD_DIR)/main

PREFIX     ?= /usr/local
LIBDIR     = $(PREFIX)/lib
INCLUDEDIR = $(PREFIX)/include

HDR_SRC_DIR := include/muda
HDR_DST_DIR := $(INCLUDEDIR)/muda

GIT_HEADER = metadata/git.h

.PHONY: all buildlib buildexe shared static clean install uninstall

cleanbuild: clean all

all: buildlib buildexe

# --- Shared Library ---
buildlib: shared static

shared: $(LIBTARGET_SO) $(GIT_HEADER)
static: $(LIBTARGET_A)  $(GIT_HEADER)

$(LIBTARGET_SO): $(OBJS_SO)
	@printf "LD     %-50s (from %d object files)\n" "$@" "$(words $^)"
	@$(CC) $(LDFLAGS_SO) -o $@ $^

$(LIBTARGET_A): $(OBJS_SO)
	@printf "AR     %-50s (from %d object files)\n" "$@" "$(words $^)"
	@$(AR) rcs $@ $^

# --- Executable ---
buildexe: $(EXETARGET)

$(EXETARGET): $(OBJS_EXE)
	@printf "LD     %-50s (from %d object files)\n" "$@" "$(words $^)"
	@$(CC) -o $@ $^ $(LDFLAGS_EXE)

# --- Compilation ---
$(OBJDIR)/%.o: %.c $(GIT_HEADER)
	@mkdir -p $(dir $@)

	@printf "CC     %-50s (from %s)\n" "$@" "$<"

	@srcdir=$(dir $<); \
	common_dirs="$(SRCDIRS_COMMON)"; \
	if echo "$$common_dirs" | grep -q "$${srcdir%%/}"; then \
		$(CC) $(CFLAGS_SO) -c $< -o $@; \
	else \
		$(CC) $(CFLAGS_EXE) -c $< -o $@; \
	fi

$(GIT_HEADER):
	@chmod +x ./scripts/gen_git_hdr.sh
	@./scripts/gen_git_hdr.sh $(GIT_HEADER)

# --- Run ---
run: $(EXETARGET)
	@printf "RUN    %-50s\n" "$(EXETARGET)"
	@$(EXETARGET)

# --- Install ---
install: $(LIBTARGET_SO) $(LIBTARGET_A)
	@for lib in $(LIBTARGET_SO) $(LIBTARGET_A); do \
		libname=$$(basename $$lib); \
		libdst=$(abspath $(LIBDIR)); \
		printf "INST   %-50s (to %s/)\n" "$$libname" "$$libdst"; \
		install -d "$$libdst"; \
		install -m 0755 "$$lib" "$$libdst/"; \
	done
	@ldconfig
	@hdrsrc=$(HDR_SRC_DIR); \
	 hdrdst=$(abspath $(HDR_DST_DIR)); \
	 printf "INST   %-50s (to %s/)\n" "headers from $$hdrsrc" "$$hdrdst"; \
	 find "$$hdrsrc" -name '*.h' | while read -r header; do \
	     rel_path="$${header#$$hdrsrc/}"; \
	     dest_dir="$$hdrdst/$$(dirname "$$rel_path")"; \
	     install -d "$$dest_dir"; \
	     install -m 0644 "$$header" "$$dest_dir/"; \
	 done

# --- Uninstall ---
uninstall:
	@for lib in $(LIBTARGET_SO) $(LIBTARGET_A); do \
		libname=$$(basename $$lib); \
		libdst=$(abspath $(LIBDIR)); \
		printf "UNST   %-50s (from %s/)\n" "$$libname" "$$libdst"; \
		rm -f "$$libdst/$$libname"; \
	done
	@ldconfig
	@printf "UNST   headers from %s/\n" "$(HDR_DST_DIR)"
	@rm -rf "$(HDR_DST_DIR)"

# --- Clean ---
clean:
	@printf "RM     %-50s (includes target)\n" "$(BUILD_DIR)/"
	@rm  -f $(GIT_HEADER)
	@rm -rf $(BUILD_DIR)/
