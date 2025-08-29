CXX=clang++
LN=clang++

SRCD=../src
BIND=../bin
INC=-I.

DEBUG?=0
MAX_LENGTH?=255

ifeq (${DEBUG},1)
CPPFLAGS= -std=c++20 -fpic -DDEBUG -g -Wall -Werror -MMD -MP -Wno-unused-value
LNFLAGS= -g
BUILDD:= ../build/debug
TARGET:= ${BIND}/main.debug
ECHO_CXX= "[CXXD] "
ECHO_LINK= "[LINKD]"
else
BUILDD=../build/release
TARGET=${BIND}/main.release
CPPFLAGS= -std=c++20 -fpic -MMD -MP 
LNFLAGS=
ECHO_CXX= "[CXX] "
ECHO_LINK= "[LINK]"
endif

ifeq (${RUNTESTS},1) 
CPPFLAGS+=-DRUNTESTS
endif

CPPFLAGS+=-DMAX_LENGTH=${MAX_LENGTH}


SOURCES=$(shell find ${SRCD} -name "*.cpp")
OBJS=$(patsubst $(SRCD)/%.cpp,$(BUILDD)/%.o,${SOURCES})
DEPS=$(patsubst $(SRCD)/%.cpp,$(BUILDD)/%.d,${SOURCES})

BUILDDIRS=$(dir ${OBJS}) 

all: release ;

${OBJS}: ${BUILDD}/%.o:%.cpp | builddirs
	@echo ${ECHO_CXX} $<
	${CXX} ${CPPFLAGS} ${INC} -c $< -o $@

release: ${OBJS} 
	@echo ${ECHO_LINK} ${TARGET}...
	${LN} ${LNFLAGS} $^ ${LIBS} -o ${TARGET}

%/: ;
	@mkdir -p $@

builddirs: $(dir ${OBJS}) ../bin;

debug-clean: ;
	@${MAKE} DEBUG=1 clean

debug: ;
	@${MAKE} DEBUG=1

test: ;
	@${MAKE} DEBUG=1 RUNTESTS=1 MAX_LENGTH=20

test-clean: ;
	@${MAKE} DEBUG=1 RUNTESTS=1 clean

clean: ;
	$(foreach f, ${OBJS}, $(shell rm ${f} 2>/dev/null))
	$(foreach d, ${DEPS}, $(shell rm ${d} 2>/dev/null))

.PHONY: all debug clean test

-include ${DEPS}

../bin: ;
	mkdir ../bin

$(foreach bdir, ${BUILDD}, $(eval $(call mkdir -p, ${bdir})))
