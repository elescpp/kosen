# H8/3052 �� ���^ Makefile
#  �菇1. �K�v�Ȑݒ��ύX���āA�Ⴄ�t�@�C�����ŕۑ����� (��Fmake-test)
#	  TARGET = , SOURCE_C = , SOURCE_ASM = ���w�肷��
#	  GDB�����[�g�f�o�b�L���O�̂Ƃ��́AREMOTE_DBG = true �Ƃ���
#	  ���̑��͒ʏ�A�ύX�̕K�v�͂Ȃ�
#  �菇2. make -f makefile�� �� make ���� (��Fmake -f make-test)

# ��������t�@�C���ƃ\�[�X�t�@�C���̎w��
# 1. ��������I�u�W�F�N�g�̃t�@�C�������w��
TARGET = no1.mot
# 2. �����ɕK�v��C�̃t�@�C�������󔒂ŋ�؂��ĕ��ׂ�
SOURCE_C = no1.c
# 3. �����ɕK�v�ȃA�Z���u���̃t�@�C�������󔒂ŋ�؂��ĕ��ׂ�
#	(�X�^�[�g�A�b�v���[�`���͏���)
SOURCE_ASM = 

# ��������I�u�W�F�N�g�̎�ނ��w��
#	(���̍��ڂ͒ʏ�ύX����K�v���Ȃ�)
#
# 1. GDB�ɂ�郊���[�g�f�o�b�L���O�w��
#	true : �w�肷��		���̑��F�w�肵�Ȃ�
REMOTE_DBG = 

# 2. RAM��f�o�b�O�܂���ROM���w�� ��
#	ram : RAM��Ŏ��s	rom : ROM��
ON_RAM = ram

# 3. �g�pRAM�̈�̎w�� ��
#	ext�FRAM�����v���O�����ƃX�^�b�N�͊O��RAM���g�p
#	     ROM�����X�^�b�N�͊O��RAM
#	int�FRAM�����v���O�����ƃX�^�b�N�͓���RAM���g�p
#	     ROM�����X�^�b�N�͓���RAM
#	�w��Ȃ��FRAM�����v���O�����͊O��RAM�A�X�^�b�N�ύX�Ȃ�
#		  ROM�����X�^�b�N�͊O��RAM
RAM_CAP = ext

# 4. GDB�ɂ��f�o�b�O���s�����ǂ����̎w�� ��
USE_GDB = true

#
# �p�X�̐ݒ�
#
CMD_PATH = /usr/local/bin
LIB_PATH = /home/class/common/H8/lib

#
# �N���X�R���p�C���֌W
#
CC = $(CMD_PATH)/h8300-hms-gcc
LD = $(CMD_PATH)/h8300-hms-ld
OBJCOPY = $(CMD_PATH)/h8300-hms-objcopy
SIZE = $(CMD_PATH)/h8300-hms-size

#
# �^�[�Q�b�g�w��
#
TARGET_COFF = $(TARGET:.mot=.coff)
MAP_FILE = $(TARGET:.mot=.map)

#
# �o�̓t�H�[�}�b�g
# binary : binary, srec : Motorola S record, ihex : Intel Hex
#
OUTPUT_FORMAT = -O srec --srec-forceS3

#
# �R���p�C���I�v�V����
#
# �C���N���[�h�f�B���N�g���̒ǉ�("*****.h"�w��̂ݗL��)
INCLUDES = -I/home/class/common/H8/include
# �R���p�C���I�v�V�����̎w��
#	-mh�FH8/300H�V���[�Y�w��
#	-mrelax�F��������R�[�h�̍œK��
#	-mint32�Fint�^�ϐ��̃r�b�g���w��
#	-O2�Fgcc�̍œK�����x���̎w��
#	-Wall�F�R���p�C�����̌x�����b�Z�[�W�̑I��(�S��)
CFLAGS = -mh -mrelax -mint32 -O2 $(INCLUDES) -Wall

#
# �w��ɍ��킹���X�^�[�g�A�b�v���[�`���ƃ����J�X�N���v�g�̑I��
#
ifeq ($(REMOTE_DBG), true)
	USE_GDB = true
	ON_RAM = ram
	RAM_CAP =
endif

ifeq ($(USE_GDB), true)
	CFLAGS := $(CFLAGS) -g
endif

ifeq ($(ON_RAM), ram)
	LDSCRIPT = $(LIB_PATH)/h8-3052-ram.x
	STARTUP = $(LIB_PATH)/ramcrt.s
	ifeq ($(RAM_CAP), int)
	     	LDSCRIPT = $(LIB_PATH)/h8-3052-ram8k.x
		STARTUP = $(LIB_PATH)/ramcrt-8k.s
	endif
	ifeq ($(RAM_CAP), ext)
		LDSCRIPT = $(LIB_PATH)/h8-3052-ram.x
		STARTUP = $(LIB_PATH)/ramcrt-ext.s
	endif
	ifeq ($(REMOTE_DBG), true)
		LDSCRIPT = $(LIB_PATH)/h8-3052-ram-dbg.x
		STARTUP = $(LIB_PATH)/ramcrt-dbg.s
	endif
else
	ifeq ($(RAM_CAP), int)
		LDSCRIPT = $(LIB_PATH)/h8-3052-rom8k.x
		STARTUP = $(LIB_PATH)/romcrt-8k.s
	else
	     	LDSCRIPT = $(LIB_PATH)/h8-3052-rom.x
		STARTUP = $(LIB_PATH)/romcrt-ext.s
	endif
endif

#
# �����N���̃R���p�C���I�v�V�����̎w��
#	-T filename�F�����J�X�N���v�g�t�@�C���̎w��
#	-nostartfiles�F�W���̃X�^�[�g�A�b�v���g�p���Ȃ�
#	-Wl,�p�����[�^,�c�F�����J�ɓn���p�����[�^�w��
#		-Map mapfilename�F�������}�b�v��mapfilename�ɏo��
LDFLAGS = -T $(LDSCRIPT) -nostartfiles -Wl,-Map,$(MAP_FILE)

#
# �I�u�W�F�N�g�̎w��
#
OBJ = $(STARTUP:.s=.o) $(SOURCE_C:.c=.o) $(SOURCE_ASM:.s=.o)

#
# �T�t�B�b�N�X���[���K�p�̊g���q�w��
#
.SUFFIXES: .c .s .o 

#
# ���[��
#
$(TARGET) : $(TARGET_COFF)
	$(OBJCOPY) -v $(OUTPUT_FORMAT) $(TARGET_COFF) $(TARGET)

$(TARGET_COFF) : $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(TARGET_COFF)
	$(SIZE) -Ax $(TARGET_COFF)

clean :
	rm -f *.o $(TARGET) $(TARGET_COFF) $(MAP_FILE)

#
# �T�t�B�b�N�X���[��
#
.c.o:
	$(CC) -c $(CFLAGS) $<
.s.o:
	$(CC) -c $(CFLAGS) $<
