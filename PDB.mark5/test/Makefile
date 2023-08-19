#APP_TYPE=MP
include $(PCF_DEV_HOME)/makehead
include $(PCF_MP_HOME)/makehead
#
GIT_VER = gitversion.h

EXE     = pdbTest
SRCS    = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
OBJS    = $(SRCS:.cpp=.o)

all :: cleancore $(EXE)

PCF_LIB += -lGDB

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
LDFLAGS += -L$(PCF_LIB_DIR)/libGDB.a
ifeq (-D__NDB_USE__,$(findstring -D__NDB_USE__,$(CFLAGS)))
LDFLAGS += -L$(PCF_LIB_DIR)/NDB_MODULE -l:libNDB-Module.a
#LDFLAGS += -L$(PCF_LIB_DIR)/RDLIB -lRDLib64 -lefence
LDFLAGS += -L$(PCF_LIB_DIR)/RDLIB -lRDLib64
endif


$(EXE) : $(GIT_VER) $(OBJS) $(HEADERS)
	@echo "[1;40;36m>>>>>>linking [$(OBJS) -> $@]<<<<<<[0m"
	$(CXX) -o $(EXE) $(OBJS) $(LDFLAGS)
	@date '+%Y.%m.%d %H:%M:%S'


$(GIT_VER): $(BIZ_HOME)/.git/HEAD $(BIZ_HOME)/.git/index 
	@echo "#define APP_GIT_REVISION \"$(shell git rev-parse --short HEAD)\"" > $@

clean:
	$(RM) $(OBJS) $(EXE) core.* *.d

cleancore:
	$(RM) core.*

DEPS := $(OBJS:.o=.d)

-include $(DEPS)
