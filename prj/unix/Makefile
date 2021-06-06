CCX=g++
C_DIR=mkdir
ECHO= echo
RM= rm

ROOT_DIR= ../..
PLAT_DIR= unix

SRC_DIR= $(ROOT_DIR)/src
INC_DIR= $(ROOT_DIR)/include

SERV_DIR= server
CL_DIR= client
FUNC_DIR= func
OBJ_DIR= $(ROOT_DIR)/obj/$(PLAT_DIR)
OUT_DIR= $(ROOT_DIR)/out/$(PLAT_DIR)
SRC_EXT= cpp
OBJ_EXT= o

SERV_LINK_OPT= -pthread
SERV_SRC_DIR= $(SRC_DIR)/$(SERV_DIR)
SERV_INC_DIR= $(INC_DIR)/$(SERV_DIR)
SERV_OBJ_DIR= $(OBJ_DIR)/$(SERV_DIR)
SERV_OUT= $(OUT_DIR)/$(SERV_DIR)

CL_LINK_OPT= -pthread
CL_SRC_DIR= $(SRC_DIR)/$(CL_DIR)
CL_INC_DIR= $(INC_DIR)/$(CL_DIR)
CL_OBJ_DIR= $(OBJ_DIR)/$(CL_DIR)
CL_OUT= $(OUT_DIR)/$(CL_DIR)

FUNC_SRC_DIR= $(SRC_DIR)/$(FUNC_DIR)
FUNC_INC_DIR= $(INC_DIR)/$(FUNC_DIR)
FUNC_OBJ_DIR= $(OBJ_DIR)/$(FUNC_DIR)

SERV_SRC_FILES= $(shell find $(SERV_SRC_DIR) -type f -name *.$(SRC_EXT))
SERV_OBJ_FILES= $(patsubst $(SERV_SRC_DIR)/%.$(SRC_EXT), $(SERV_OBJ_DIR)/%.$(OBJ_EXT), $(SERV_SRC_FILES))

CL_SRC_FILES= $(shell find $(CL_SRC_DIR) -type f -name *.$(SRC_EXT))
CL_OBJ_FILES= $(patsubst $(CL_SRC_DIR)/%.$(SRC_EXT), $(CL_OBJ_DIR)/%.$(OBJ_EXT), $(CL_SRC_FILES))

FUNC_SRC_FILES= $(shell find $(FUNC_SRC_DIR) -type f -name *.$(SRC_EXT))
FUNC_OBJ_FILES= $(patsubst $(FUNC_SRC_DIR)/%.$(SRC_EXT), $(FUNC_OBJ_DIR)/%.$(OBJ_EXT), $(FUNC_SRC_FILES))

all: $(SERV_OUT) $(CL_OUT)
	@$(ECHO) server built to $(SERV_OUT)
	@$(ECHO) client built to $(CL_OUT)

$(SERV_OUT): $(SERV_OBJ_FILES) $(FUNC_OBJ_FILES)
	@$(C_DIR) -p $(OUT_DIR)
	@$(ECHO) linking $(SERV_OBJ_FILES) to $(SERV_OUT)
	@$(CXX) -o $(SERV_OUT) $(SERV_OBJ_FILES) $(FUNC_OBJ_FILES) $(SERV_LINK_OPT)

$(CL_OUT): $(CL_OBJ_FILES) $(FUNC_OBJ_FILES)
	@$(C_DIR) -p $(OUT_DIR)
	@$(ECHO) linking $(CL_OBJ_FILES) to $(CL_OUT)
	@$(CXX) -o $(CL_OUT) $(CL_OBJ_FILES) $(FUNC_OBJ_FILES) $(CL_LINK_OPT)

$(SERV_OBJ_FILES): $(SERV_OBJ_DIR)/%.$(OBJ_EXT): $(SERV_SRC_DIR)/%.$(SRC_EXT)
	@$(C_DIR) -p $(SERV_OBJ_DIR)
	@$(ECHO) compiling $< to $@
	@$(CXX) -c -o $@ $< -I$(SERV_INC_DIR) -I$(FUNC_INC_DIR)

$(CL_OBJ_FILES): $(CL_OBJ_DIR)/%.$(OBJ_EXT): $(CL_SRC_DIR)/%.$(SRC_EXT)
	@$(C_DIR) -p $(CL_OBJ_DIR)
	@$(ECHO) compiling $< to $@
	@$(CXX) -c -o $@ $< -I$(CL_INC_DIR) -I$(FUNC_INC_DIR)

$(FUNC_OBJ_FILES): $(FUNC_OBJ_DIR)/%.$(OBJ_EXT): $(FUNC_SRC_DIR)/%.$(SRC_EXT)
	@$(C_DIR) -p $(FUNC_OBJ_DIR)
	@$(ECHO) compiling $< to $@
	@$(CXX) -c -o $@ $< -I$(FUNC_INC_DIR)

$(SERV_DIR): $(SERV_OUT)
	@$(ECHO) server built to $(SERV_OUT)

$(CL_DIR): $(CL_OUT)
	@$(ECHO) client built to $(CL_OUT)

$(FUNC_DIR): $(FUNC_OBJ_FILES)
	@$(ECHO) func objs built to $(FUNC_OBJ_DIR)

clean: cleanobj
	@$(RM) -r $(OUT_DIR)
	@$(ECHO) removing $(OUT_DIR)

cleanobj:
	@$(RM) -r $(OBJ_DIR)
	@$(ECHO) removing $(OBJ_DIR)
