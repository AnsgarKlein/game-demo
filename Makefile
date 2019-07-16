CC         :=  clang
CXX        :=  clang++

SOURCEDIR  :=  src
TMPDIR     :=  tmp
BUILDDIR   :=  build
SCRIPTDIR  :=  scripts
SPRITEDIR  :=  sprites
DOCDIR     :=  doc/doc

DOCCONFIG  :=  doc/Doxygen.config

TARGET     :=  demo
PRINTSCRIPT := print.sh

#IMAGES    :=  dirt.png
#IMAGES    +=  sand.png
#IMAGES    +=  box.png
#IMAGES    +=  player.png
#IMAGES    +=  target.png
#IMAGES    +=  wall.png
#IMAGES    +=  woodplanks.png
#IMAGES    +=  tree.png
IMAGES     := $(notdir $(wildcard $(SPRITEDIR)/*.png))

SOURCES    :=  $(notdir $(wildcard $(SOURCEDIR)/*.cpp))

HEADERS    :=  $(notdir $(wildcard $(SOURCEDIR)/*.h))

OBJECTS    :=  $(SOURCES)
OBJECTS    :=  $(OBJECTS:.cpp=.o)

LIBRARIES  :=  sdl2
LIBRARIES  +=  SDL2_image

FLAGS      +=  -Wall -Wextra -Wpedantic

CXXFLAGS   :=  -x c++ -std=c++11
CXXFLAGS   +=  -Wall -Wextra -Wpedantic
CXXFLAGS   +=  $(shell pkg-config --cflags $(LIBRARIES))

LDFLAGS    :=  $(FLAGS)
LDFLAGS    +=  $(shell pkg-config --libs $(LIBRARIES))

.PHONY: all clean doc debug optimized

all: $(BUILDDIR)/$(TARGET)
all: $(addprefix $(SOURCEDIR)/, $(SOURCES))
all: $(addprefix $(SOURCEDIR)/, $(HEADERS))
all: $(addprefix $(BUILDDIR)/sprites/, $(IMAGES))
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

$(BUILDDIR)/$(SPRITEDIR)/%.png: $(SPRITEDIR)/%.png | $(BUILDDIR)/$(SPRITEDIR)
	@/bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [GEN]     $(notdir $@)"
	@cp $< $@

$(TMPDIR)/%.o: $(SOURCEDIR)/%.cpp $(addprefix $(SOURCEDIR)/, $(HEADERS)) | $(TMPDIR)
#$(TMPDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h			# this does not always work
	@/bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [CC]      $(notdir $@)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/$(SPRITEDIR): | $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/$(SPRITEDIR)

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
	@for img in $(wildcard $(BUILDDIR)/$(SPRITEDIR)/*.png); do \
	  /bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [CLEAN]   $$img" ; \
	  rm -f $$img ; \
	done
	
	@# Remove object files
	@for obj in $(wildcard $(TMPDIR)/*.o); do \
	  /bin/sh $(SCRIPTDIR)/$(PRINTSCRIPT) " [CLEAN]   $$obj" ; \
	  rm -f $$obj ; \
	done
	
