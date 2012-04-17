SRCS = test.c

TARGET_HEX = test.hex

PART = 33FJ64MC804

PARTFAMILY = dsPIC33F

# Set this to wherever you have installed the toolchain
PIC30 = /home/kartikmohta/temp/c30

CC = pic30-coff-gcc
AS = pic30-coff-as
LD = pic30-coff-ld
BX = pic30-coff-bin2hex

CFLAGS = -O2 -g -Wall
LDFLAGS = --heap=0

HEADERS = $(wildcard *.h)

INCLUDEPATH = -I$(PIC30)/pic30-nonfree/include \
	      -I$(PIC30)/pic30-nonfree/support/$(PARTFAMILY)/h

LIBPATH = -L$(PIC30)/pic30-nonfree/lib \
  	  -L$(PIC30)/pic30-nonfree/lib/$(PARTFAMILY)

LIBS = -lc -lm -lpic30 \
       -lp$(subst f,F,$(PART))

LINKERSCRIPT = $(PIC30)/pic30-nonfree/support/$(PARTFAMILY)/gld/p$(PART).gld

TARGET_COFF = $(patsubst %.hex, %.coff, $(TARGET_HEX))
OBJS = $(patsubst %.c, %.o, $(SRCS))

$(TARGET_HEX): $(TARGET_COFF)
	@echo "  [BX]  $@"
	@$(BX) $(TARGET_COFF)

$(TARGET_COFF): $(OBJS)
	@echo "  [LD]  $@"
	@$(LD) $(LDFLAGS) -p$(PART) $(LIBPATH) $^ $(LIBS) -T$(LINKERSCRIPT) -o $@

%.o:	%.c $(HEADERS)
	@echo "  [CC]  $@"
	@$(CC) $(CFLAGS) -mcpu=$(PART) $(INCLUDEPATH) -c $^ -o $@

clean:
	$(RM) *.o *.coff *~
