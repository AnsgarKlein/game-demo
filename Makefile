SOURCEDIR  :=  src
TMPDIR     :=  tmp
BUILDDIR   :=  build
SCRIPTDIR  :=  scripts
SPRITESDIR :=  sprites
DOCDIR     :=  doc/doc

DOCCONFIG  :=  doc/Doxygen.config

TARGET     :=  demo
PRINTSCRIPT := print.sh

SPRITES    := $(notdir $(wildcard $(SPRITESDIR)/*.png))
SPRITES    += $(notdir $(wildcard $(SPRITESDIR)/*.json))

SOURCES    :=  $(notdir $(wildcard $(SOURCEDIR)/*.cpp))

HEADERS    :=  $(notdir $(wildcard $(SOURCEDIR)/*.h))

OBJECTS    :=  $(SOURCES)
OBJECTS    :=  $(OBJECTS:.cpp=.o)

LIBRARIES  :=  sdl2
LIBRARIES  +=  SDL2_image

FLAGS      +=  -Wall -Wextra -Wpedantic

CXXFLAGS   :=  -x c++ -std=c++17
CXXFLAGS   +=  -Wall -Wextra -Wpedantic
CXXFLAGS   +=  $(shell pkg-config --cflags $(LIBRARIES))

LDFLAGS    :=  $(FLAGS)
LDFLAGS    +=  $(shell pkg-config --libs $(LIBRARIES))

.PHONY: all clean doc debug optimized

all: $(BUILDDIR)/$(TARGET)
all: $(addprefix $(BUILDDIR)/sprites/, $(SPRITES))
all: $(addprefix $(SOURCEDIR)/, $(HEADERS))
all: $(addprefix $(SOURCEDIR)/, $(SOURCES))
	@#

debug: CXXFLAGS += -g
debug: LDFLAGS += -g
debug: all

optimized: CXXFLAGS += -O3
optimized: LDFLAGS += -O3
optimized: all

profiling: CXXFLAGS += -pg
profiling: LDFLAGS += -pg
profiling: all

doc:
	doxygen $(DOCCONFIG)

$(BUILDDIR)/$(TARGET): $(addprefix $(TMPDIR)/, $(OBJECTS)) | $(BUILDDIR)
	@/bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [LD]      $(notdir $@)"
	@$(CXX) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/$(SPRITESDIR)/%.png: $(SPRITESDIR)/%.png | $(BUILDDIR)/$(SPRITESDIR)
	@/bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [GEN]     $(notdir $@)"
	@cp $< $@

$(BUILDDIR)/$(SPRITESDIR)/%.json: $(SPRITESDIR)/%.json | $(BUILDDIR)/$(SPRITESDIR)
	@/bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [GEN]     $(notdir $@)"
	@cp $< $@

$(TMPDIR)/%.o: $(SOURCEDIR)/%.cpp $(addprefix $(SOURCEDIR)/, $(HEADERS)) | $(TMPDIR)
	@/bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [CC]      $(notdir $@)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/$(SPRITESDIR): | $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/$(SPRITESDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(TMPDIR):
	@mkdir -p $(TMPDIR)

clean: 
	@# Remove executable
	@if [[ -f $(BUILDDIR)/$(TARGET) ]]; then \
	  /bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [CLEAN]   $(BUILDDIR)/$(TARGET)" ; \
	  rm -f $(BUILDDIR)/$(TARGET) ; \
	fi

	@# Remove sprites
	@for img in $(wildcard $(BUILDDIR)/$(SPRITESDIR)/*.png); do \
	  /bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [CLEAN]   $$img" ; \
	  rm -f $$img ; \
	done

	@for json in $(wildcard $(BUILDDIR)/$(SPRITESDIR)/*.json); do \
	  /bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [CLEAN]   $$json" ; \
	  rm -f $$json ; \
	done
	
	@# Remove object files
	@for obj in $(wildcard $(TMPDIR)/*.o); do \
	  /bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [CLEAN]   $$obj" ; \
	  rm -f $$obj ; \
	done
	
