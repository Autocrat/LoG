TARGET=grid

SRCS=\
main.cpp \
Room.cpp \
Block.cpp \
Portal.cpp \
EventHandler.cpp \
ShaderUtil.cpp \
BlockMapKey.cpp \
Shaders.cpp \
Ship.cpp \
NodeData.cpp \


OBJECTS=$(SRCS:.cpp=.o)

INCLUDE=\
-I/usr/include/GL \
-I/usr/include \
-I/usr/local/include \


LIBDIR=\
-L/usr/local/lib \
-L/usr/lib \

LIBRARIES=\
-lSDL2main \
-lSDL2 \
-lSDL2_image \
-lGL \
-lbox2d \
-ltinyxml2 \


#-lglew \
-lopengl \
-lBox2d \
-ltinyxml2\


#-mwindows \


DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
CPPFLAGS += $(INCLUDE)
#CPPFLAGS += -DWINDOWS
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d


%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
	echo

$(TARGET) : $(OBJECTS)
	g++ -o $(TARGET)  $(OBJECTS) $(LIBDIR) $(LIBRARIES) 

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

clean:
	rm $(OBJECTS) $(TARGET)

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

