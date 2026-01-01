CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Shader compilation
GLSLC = glslc
VERT_SHADERS = $(wildcard shaders/*.vert)
FRAG_SHADERS = $(wildcard shaders/*.frag)
VERT_SPV = $(VERT_SHADERS:.vert=.vert.spv)
FRAG_SPV = $(FRAG_SHADERS:.frag=.frag.spv)
SHADERS = $(VERT_SPV) $(FRAG_SPV)

DIST_DIR = dist
TARGET = $(DIST_DIR)/VulkanTest

$(TARGET): *.cpp *.hpp $(SHADERS) | $(DIST_DIR)
	g++ $(CFLAGS) -o $(TARGET) *.cpp $(LDFLAGS)

shaders/%.vert.spv: shaders/%.vert
	$(GLSLC) $< -o $@

shaders/%.frag.spv: shaders/%.frag
	$(GLSLC) $< -o $@

$(DIST_DIR):
	mkdir -p $(DIST_DIR)

.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(DIST_DIR)