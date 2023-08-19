#APP_TYPE=MP
include $(PCF_DEV_HOME)/makehead
include $(PCF_MP_HOME)/makehead
#
GIT_VER = gitversion.h

LIB     = libGDB.a
S_LIB     =libCOMM.so
SRCS    = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
OBJS    = $(SRCS:.cpp=.o)

all :: $(LIB)

 CFLAGS += -D__ATOM_TRANSFER__
 CFLAGS += -D__DEBUG_DATE__
 CFLAGS += -D__USED_NEW_HTTP2CLI__
 CFLAGS += -U__USED_SUBS_EXCEPT_MATCHED__
 CFLAGS += -D__USE_CQUEUE__
 CFLAGS += -D__DURATION_CHK__
 CFLAGS += -U__MY5G_MODE__
 CFLAGS += -D__NDB_USE__
 CFLAGS += -U__USE_BINDING_SESSION__
 CFLAGS += -D__UDR_NOTIFY__
ifeq (,$(findstring -D__NDB_USE__,$(CFLAGS)))
 CFLAGS += -D__USE_SUBS_CACHE__
 CFLAGS += -D__USE_SESS_CACHE__
endif
ifeq (-D__NDB_USE__,$(findstring -D__NDB_USE__,$(CFLAGS)))
LDFLAGS += -L$(PCF_LIB_DIR)/NDB_MODULE -l:libNDB-Module.a
LDFLAGS += -L$(PCF_LIB_DIR)/RDLIB -lRDLib64
endif


$(EXE) : $(GIT_VER) $(OBJS) $(HEADERS)
	@echo "[1;40;36m>>>>>>linking [$(OBJS) -> $@]<<<<<<[0m"
	$(CXX) -o $(EXE) $(OBJS) $(LDFLAGS)
	@date '+%Y.%m.%d %H:%M:%S'

$(LIB) : $(OBJS) $(HEADERS) 
	$(AR) $(AR_OPTION) $(LIB) $(OBJS) 
	@$(MAKE) install

$(S_LIB) : $(OBJS) $(HEADERS) 
	$(CXX) -shared -o $(S_LIB) $(OBJS) 
	@$(MAKE) install_so

$(GIT_VER): $(BIZ_HOME)/.git/HEAD $(BIZ_HOME)/.git/index 
	@echo "#define APP_GIT_REVISION \"$(shell git rev-parse --short HEAD)\"" > $@

install:
	@if [ ! -d "$(PCF_LIB_DIR)" ]; then mkdir -p "$(PCF_LIB_DIR)";  fi
	$(CP) $(LIB) $(PCF_LIB_DIR)/$(LIB)
	@if [ ! -d "$(PCF_INC_DIR)" ]; then mkdir -p "$(PCF_INC_DIR)";  fi
	$(CP) $(HEADERS) $(PCF_INC_DIR)/

install_so:
	@if [ ! -d "$(PCF_LIB_DIR)" ]; then mkdir -p "$(PCF_LIB_DIR)";  fi
	$(CP) $(S_LIB) $(PCF_LIB_DIR)/$(S_LIB)
	@if [ ! -d "$(PCF_INC_DIR)" ]; then mkdir -p "$(PCF_INC_DIR)";  fi
	$(CP) $(HEADERS) $(PCF_INC_DIR)/


clean:
	$(RM) $(OBJS) $(EXE) core.* *.d

DEPS := $(OBJS:.o=.d)

-include $(DEPS)
