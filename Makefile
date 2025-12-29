CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

DIST_DIR = dist
TARGET = $(DIST_DIR)/VulkanTest

$(TARGET): *.cpp *.hpp | $(DIST_DIR)
	g++ $(CFLAGS) -o $(TARGET) *.cpp $(LDFLAGS)

$(DIST_DIR):
	mkdir -p $(DIST_DIR)

.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(DIST_DIR)