# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_run
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_run)
  Daedalus_Core_config = debug_run
  Daedalus_Entropia_config = debug_run

else ifeq ($(config),debug_static-build)
  Daedalus_Core_config = debug_static-build
  Daedalus_Entropia_config = debug_static-build

else ifeq ($(config),debug_dynamic-build)
  Daedalus_Core_config = debug_dynamic-build
  Daedalus_Entropia_config = debug_dynamic-build

else ifeq ($(config),release_run)
  Daedalus_Core_config = release_run
  Daedalus_Entropia_config = release_run

else ifeq ($(config),release_static-build)
  Daedalus_Core_config = release_static-build
  Daedalus_Entropia_config = release_static-build

else ifeq ($(config),release_dynamic-build)
  Daedalus_Core_config = release_dynamic-build
  Daedalus_Entropia_config = release_dynamic-build

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := Daedalus-Core Daedalus-Entropia

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

Daedalus-Core:
ifneq (,$(Daedalus_Core_config))
	@echo "==== Building Daedalus-Core ($(Daedalus_Core_config)) ===="
	@${MAKE} --no-print-directory -C daedalus-core/src/build/daedalus-core -f Makefile config=$(Daedalus_Core_config)
endif

Daedalus-Entropia: Daedalus-Core
ifneq (,$(Daedalus_Entropia_config))
	@echo "==== Building Daedalus-Entropia ($(Daedalus_Entropia_config)) ===="
	@${MAKE} --no-print-directory -C build/daedalus-entropia -f Makefile config=$(Daedalus_Entropia_config)
endif

clean:
	@${MAKE} --no-print-directory -C daedalus-core/src/build/daedalus-core -f Makefile clean
	@${MAKE} --no-print-directory -C build/daedalus-entropia -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_run"
	@echo "  debug_static-build"
	@echo "  debug_dynamic-build"
	@echo "  release_run"
	@echo "  release_static-build"
	@echo "  release_dynamic-build"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   Daedalus-Core"
	@echo "   Daedalus-Entropia"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"