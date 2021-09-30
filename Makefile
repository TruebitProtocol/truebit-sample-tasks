app = tb_mapreduce_task
lib = libmap.so libreduce.so

CFLAGS = -Wall -ansi -pedantic
LDFLAGS = -ldl

all: $(app) $(lib)

lib%.so: %.c
    $(GCC) -shared -fPIC $(CFLAGS) -o $@ $<

clean:
    $(RM) $(app) $(lib)

run: ./$(app)

$(lib): map.h reduce.h
