# Variáveis de Configuração
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g # C++11, warnings adicionais, e informações de debug
INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/tp3.out

# Obtém todos os arquivos .cpp do diretório src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Converte os nomes dos arquivos .cpp para .o para o diretório obj
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# O objetivo 'all' é o padrão e constrói o executável final
all: dirs $(TARGET)

# Cria os diretórios bin e obj se não existirem
dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

# Regra para o executável final
$(TARGET): $(OBJS)
	@echo "Linking executable: $@"
	$(CXX) $(OBJS) -o $@

# Regra para compilar arquivos .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $< to $@"
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Limpa os arquivos gerados pela compilação
clean:
	@echo "Cleaning up..."
	@rm -f $(OBJ_DIR)/*.o
	@rm -f $(TARGET)
	@rmdir $(OBJ_DIR) 2>/dev/null || true
	@rmdir $(BIN_DIR) 2>/dev/null || true

# Garante que 'clean' e 'dirs' são objetivos falsos e não arquivos
.PHONY: all clean dirs