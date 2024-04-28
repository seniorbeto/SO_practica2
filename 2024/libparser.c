typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned char    dwfenc;
typedef unsigned int    dword;
typedef unsigned long    qword;
typedef unsigned long    ulong;
typedef unsigned char    undefined1;
typedef unsigned int    undefined4;
typedef unsigned long    undefined8;
typedef unsigned short    word;
typedef struct eh_frame_hdr eh_frame_hdr, *Peh_frame_hdr;

struct eh_frame_hdr {
    byte eh_frame_hdr_version; // Exception Handler Frame Header Version
    dwfenc eh_frame_pointer_encoding; // Exception Handler Frame Pointer Encoding
    dwfenc eh_frame_desc_entry_count_encoding; // Encoding of # of Exception Handler FDEs
    dwfenc eh_frame_table_encoding; // Exception Handler Table Encoding
};

typedef struct NoteGnuPropertyElement_4 NoteGnuPropertyElement_4, *PNoteGnuPropertyElement_4;

struct NoteGnuPropertyElement_4 {
    dword prType;
    dword prDatasz;
    byte data[4];
};

typedef struct fde_table_entry fde_table_entry, *Pfde_table_entry;

struct fde_table_entry {
    dword initial_loc; // Initial Location
    dword data_loc; // Data location
};

typedef ulong size_t;

typedef long __ssize_t;

typedef __ssize_t ssize_t;

typedef enum Elf_ProgramHeaderType {
    PT_NULL=0,
    PT_LOAD=1,
    PT_DYNAMIC=2,
    PT_INTERP=3,
    PT_NOTE=4,
    PT_SHLIB=5,
    PT_PHDR=6,
    PT_TLS=7,
    PT_GNU_EH_FRAME=1685382480,
    PT_GNU_STACK=1685382481,
    PT_GNU_RELRO=1685382482
} Elf_ProgramHeaderType;

typedef struct Elf64_Dyn Elf64_Dyn, *PElf64_Dyn;

typedef enum Elf64_DynTag {
    DT_NULL=0,
    DT_NEEDED=1,
    DT_PLTRELSZ=2,
    DT_PLTGOT=3,
    DT_HASH=4,
    DT_STRTAB=5,
    DT_SYMTAB=6,
    DT_RELA=7,
    DT_RELASZ=8,
    DT_RELAENT=9,
    DT_STRSZ=10,
    DT_SYMENT=11,
    DT_INIT=12,
    DT_FINI=13,
    DT_SONAME=14,
    DT_RPATH=15,
    DT_SYMBOLIC=16,
    DT_REL=17,
    DT_RELSZ=18,
    DT_RELENT=19,
    DT_PLTREL=20,
    DT_DEBUG=21,
    DT_TEXTREL=22,
    DT_JMPREL=23,
    DT_BIND_NOW=24,
    DT_INIT_ARRAY=25,
    DT_FINI_ARRAY=26,
    DT_INIT_ARRAYSZ=27,
    DT_FINI_ARRAYSZ=28,
    DT_RUNPATH=29,
    DT_FLAGS=30,
    DT_PREINIT_ARRAY=32,
    DT_PREINIT_ARRAYSZ=33,
    DT_RELRSZ=35,
    DT_RELR=36,
    DT_RELRENT=37,
    DT_ANDROID_REL=1610612751,
    DT_ANDROID_RELSZ=1610612752,
    DT_ANDROID_RELA=1610612753,
    DT_ANDROID_RELASZ=1610612754,
    DT_ANDROID_RELR=1879040000,
    DT_ANDROID_RELRSZ=1879040001,
    DT_ANDROID_RELRENT=1879040003,
    DT_GNU_PRELINKED=1879047669,
    DT_GNU_CONFLICTSZ=1879047670,
    DT_GNU_LIBLISTSZ=1879047671,
    DT_CHECKSUM=1879047672,
    DT_PLTPADSZ=1879047673,
    DT_MOVEENT=1879047674,
    DT_MOVESZ=1879047675,
    DT_FEATURE_1=1879047676,
    DT_POSFLAG_1=1879047677,
    DT_SYMINSZ=1879047678,
    DT_SYMINENT=1879047679,
    DT_GNU_XHASH=1879047924,
    DT_GNU_HASH=1879047925,
    DT_TLSDESC_PLT=1879047926,
    DT_TLSDESC_GOT=1879047927,
    DT_GNU_CONFLICT=1879047928,
    DT_GNU_LIBLIST=1879047929,
    DT_CONFIG=1879047930,
    DT_DEPAUDIT=1879047931,
    DT_AUDIT=1879047932,
    DT_PLTPAD=1879047933,
    DT_MOVETAB=1879047934,
    DT_SYMINFO=1879047935,
    DT_VERSYM=1879048176,
    DT_RELACOUNT=1879048185,
    DT_RELCOUNT=1879048186,
    DT_FLAGS_1=1879048187,
    DT_VERDEF=1879048188,
    DT_VERDEFNUM=1879048189,
    DT_VERNEED=1879048190,
    DT_VERNEEDNUM=1879048191,
    DT_AUXILIARY=2147483645,
    DT_FILTER=2147483647
} Elf64_DynTag;

struct Elf64_Dyn {
    enum Elf64_DynTag d_tag;
    qword d_val;
};

typedef struct Elf64_Shdr Elf64_Shdr, *PElf64_Shdr;

typedef enum Elf_SectionHeaderType {
    SHT_NULL=0,
    SHT_PROGBITS=1,
    SHT_SYMTAB=2,
    SHT_STRTAB=3,
    SHT_RELA=4,
    SHT_HASH=5,
    SHT_DYNAMIC=6,
    SHT_NOTE=7,
    SHT_NOBITS=8,
    SHT_REL=9,
    SHT_SHLIB=10,
    SHT_DYNSYM=11,
    SHT_INIT_ARRAY=14,
    SHT_FINI_ARRAY=15,
    SHT_PREINIT_ARRAY=16,
    SHT_GROUP=17,
    SHT_SYMTAB_SHNDX=18,
    SHT_ANDROID_REL=1610612737,
    SHT_ANDROID_RELA=1610612738,
    SHT_GNU_ATTRIBUTES=1879048181,
    SHT_GNU_HASH=1879048182,
    SHT_GNU_LIBLIST=1879048183,
    SHT_CHECKSUM=1879048184,
    SHT_SUNW_move=1879048186,
    SHT_SUNW_COMDAT=1879048187,
    SHT_SUNW_syminfo=1879048188,
    SHT_GNU_verdef=1879048189,
    SHT_GNU_verneed=1879048190,
    SHT_GNU_versym=1879048191
} Elf_SectionHeaderType;

struct Elf64_Shdr {
    dword sh_name;
    enum Elf_SectionHeaderType sh_type;
    qword sh_flags;
    qword sh_addr;
    qword sh_offset;
    qword sh_size;
    dword sh_link;
    dword sh_info;
    qword sh_addralign;
    qword sh_entsize;
};

typedef struct Elf64_Sym Elf64_Sym, *PElf64_Sym;

struct Elf64_Sym {
    dword st_name;
    byte st_info;
    byte st_other;
    word st_shndx;
    qword st_value;
    qword st_size;
};

typedef struct GnuBuildId GnuBuildId, *PGnuBuildId;

struct GnuBuildId {
    dword namesz; // Length of name field
    dword descsz; // Length of description field
    dword type; // Vendor specific type
    char name[4]; // Vendor name
    byte hash[20];
};

typedef struct NoteGnuProperty_4 NoteGnuProperty_4, *PNoteGnuProperty_4;

struct NoteGnuProperty_4 {
    dword namesz; // Length of name field
    dword descsz; // Length of description field
    dword type; // Vendor specific type
    char name[4]; // Vendor name
};

typedef struct Elf64_Ehdr Elf64_Ehdr, *PElf64_Ehdr;

struct Elf64_Ehdr {
    byte e_ident_magic_num;
    char e_ident_magic_str[3];
    byte e_ident_class;
    byte e_ident_data;
    byte e_ident_version;
    byte e_ident_osabi;
    byte e_ident_abiversion;
    byte e_ident_pad[7];
    word e_type;
    word e_machine;
    dword e_version;
    qword e_entry;
    qword e_phoff;
    qword e_shoff;
    dword e_flags;
    word e_ehsize;
    word e_phentsize;
    word e_phnum;
    word e_shentsize;
    word e_shnum;
    word e_shstrndx;
};

typedef struct Elf64_Rela Elf64_Rela, *PElf64_Rela;

struct Elf64_Rela {
    qword r_offset; // location to apply the relocation action
    qword r_info; // the symbol table index and the type of relocation
    qword r_addend; // a constant addend used to compute the relocatable field value
};

typedef struct Elf64_Phdr Elf64_Phdr, *PElf64_Phdr;

struct Elf64_Phdr {
    enum Elf_ProgramHeaderType p_type;
    dword p_flags;
    qword p_offset;
    qword p_vaddr;
    qword p_paddr;
    qword p_filesz;
    qword p_memsz;
    qword p_align;
};

typedef struct evp_pkey_ctx_st evp_pkey_ctx_st, *Pevp_pkey_ctx_st;

struct evp_pkey_ctx_st {
};

typedef struct evp_pkey_ctx_st EVP_PKEY_CTX;




int _init(EVP_PKEY_CTX *ctx)

{
  int iVar1;
  
  iVar1 = __gmon_start__();
  return iVar1;
}



void FUN_00101020(void)

{
  (*(code *)(undefined *)0x0)();
  return;
}



void FUN_00101120(void)

{
  __cxa_finalize();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void free(void *__ptr)

{
  free(__ptr);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int * __errno_location(void)

{
  int *piVar1;
  
  piVar1 = __errno_location();
  return piVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

char * strcpy(char *__dest,char *__src)

{
  char *pcVar1;
  
  pcVar1 = strcpy(__dest,__src);
  return pcVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int puts(char *__s)

{
  int iVar1;
  
  iVar1 = puts(__s);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

size_t strlen(char *__s)

{
  size_t sVar1;
  
  sVar1 = strlen(__s);
  return sVar1;
}



void __stack_chk_fail(void)

{
                    // WARNING: Subroutine does not return
  __stack_chk_fail();
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int printf(char *__format,...)

{
  int iVar1;
  
  iVar1 = printf(__format);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

char * strtok_r(char *__s,char *__delim,char **__save_ptr)

{
  char *pcVar1;
  
  pcVar1 = strtok_r(__s,__delim,__save_ptr);
  return pcVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ssize_t read(int __fd,void *__buf,size_t __nbytes)

{
  ssize_t sVar1;
  
  sVar1 = read(__fd,__buf,__nbytes);
  return sVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int strcmp(char *__s1,char *__s2)

{
  int iVar1;
  
  iVar1 = strcmp(__s1,__s2);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void * malloc(size_t __size)

{
  void *pvVar1;
  
  pvVar1 = malloc(__size);
  return pvVar1;
}



int tokenize_command(char *param_1)

{
  int iVar1;
  size_t sVar2;
  long in_FS_OFFSET;
  char *apcStack_40 [2];
  int iStack_30;
  int iStack_2c;
  int iStack_28;
  int iStack_24;
  char *pcStack_20;
  char *pcStack_18;
  long lStack_10;
  
  lStack_10 = *(long *)(in_FS_OFFSET + 0x28);
  iStack_30 = 0;
  iStack_2c = 0;
  iStack_28 = 0;
  apcStack_40[0] = param_1;
  pcStack_20 = strtok_r(param_1,"\n",apcStack_40);
  pcStack_18 = strtok_r(pcStack_20," ",&pcStack_20);
  iStack_24 = 0;
  while (pcStack_18 != (char *)0x0) {
    strcpy(array + (long)iStack_30 * 0x40,pcStack_18);
    iStack_2c = iStack_2c + 1;
    sVar2 = strlen(pcStack_18);
    iStack_28 = iStack_28 + (int)sVar2;
    iVar1 = strcmp(pcStack_18,"|");
    if (iVar1 == 0) {
      *(int *)(args_commands + (long)num_commands * 4) = iStack_2c;
      *(int *)(args_sizes + (long)num_commands * 4) = iStack_28;
      iStack_28 = 0;
      iStack_2c = 0;
      num_commands = num_commands + 1;
    }
    else {
      iVar1 = strcmp(pcStack_18,"<");
      if ((((iVar1 == 0) || (iVar1 = strcmp(pcStack_18,">"), iVar1 == 0)) ||
          (iVar1 = strcmp(pcStack_18,"!>"), iVar1 == 0)) ||
         (iVar1 = strcmp(pcStack_18,"&"), iVar1 == 0)) {
        if (iStack_24 == 0) {
          *(int *)(args_commands + (long)num_commands * 4) = iStack_2c;
          *(int *)(args_sizes + (long)num_commands * 4) = iStack_28;
          num_commands = num_commands + 1;
          iStack_24 = 1;
        }
        iStack_28 = 0;
        iStack_2c = 0;
      }
      else if ((*pcStack_20 == '\0') && (iStack_24 == 0)) {
        *(int *)(args_commands + (long)num_commands * 4) = iStack_2c + 1;
        *(int *)(args_sizes + (long)num_commands * 4) = iStack_28;
        iStack_28 = 0;
        iStack_2c = 0;
        num_commands = num_commands + 1;
      }
    }
    iStack_30 = iStack_30 + 1;
    pcStack_18 = strtok_r(pcStack_20," ",&pcStack_20);
  }
  if (lStack_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return iStack_30;
  }
                    // WARNING: Subroutine does not return
  __stack_chk_fail();
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void perror(char *__s)

{
  perror(__s);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int sprintf(char *__s,char *__format,...)

{
  int iVar1;
  
  iVar1 = sprintf(__s,__format);
  return iVar1;
}



void FUN_00101210(int param_1)

{
                    // WARNING: Subroutine does not return
  exit(param_1);
}



// WARNING: Removing unreachable block (ram,0x00101233)
// WARNING: Removing unreachable block (ram,0x0010123f)

void deregister_tm_clones(void)

{
  return;
}



// WARNING: Removing unreachable block (ram,0x00101274)
// WARNING: Removing unreachable block (ram,0x00101280)

void register_tm_clones(void)

{
  return;
}



void __do_global_dtors_aux(void)

{
  if (completed_0 != '\0') {
    return;
  }
  FUN_00101120(__dso_handle);
  deregister_tm_clones();
  completed_0 = 1;
  return;
}



void frame_dummy(void)

{
  register_tm_clones();
  return;
}



int tokenize_command(char *param_1)

{
  int iVar1;
  size_t sVar2;
  long in_FS_OFFSET;
  char *local_40 [2];
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  char *local_20;
  char *local_18;
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_40[0] = param_1;
  local_20 = strtok_r(param_1,"\n",local_40);
  local_18 = strtok_r(local_20," ",&local_20);
  local_24 = 0;
  do {
    if (local_18 == (char *)0x0) {
      if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    // WARNING: Subroutine does not return
        __stack_chk_fail();
      }
      return local_30;
    }
    strcpy(array + (long)local_30 * 0x40,local_18);
    local_2c = local_2c + 1;
    sVar2 = strlen(local_18);
    local_28 = local_28 + (int)sVar2;
    iVar1 = strcmp(local_18,"|");
    if (iVar1 == 0) {
      *(int *)(args_commands + (long)num_commands * 4) = local_2c;
      *(int *)(args_sizes + (long)num_commands * 4) = local_28;
      local_28 = 0;
      local_2c = 0;
      num_commands = num_commands + 1;
    }
    else {
      iVar1 = strcmp(local_18,"<");
      if (iVar1 != 0) {
        iVar1 = strcmp(local_18,">");
        if (iVar1 != 0) {
          iVar1 = strcmp(local_18,"!>");
          if (iVar1 != 0) {
            iVar1 = strcmp(local_18,"&");
            if (iVar1 != 0) {
              if ((*local_20 == '\0') && (local_24 == 0)) {
                *(int *)(args_commands + (long)num_commands * 4) = local_2c + 1;
                *(int *)(args_sizes + (long)num_commands * 4) = local_28;
                local_28 = 0;
                local_2c = 0;
                num_commands = num_commands + 1;
              }
              goto LAB_00101541;
            }
          }
        }
      }
      if (local_24 == 0) {
        *(int *)(args_commands + (long)num_commands * 4) = local_2c;
        *(int *)(args_sizes + (long)num_commands * 4) = local_28;
        num_commands = num_commands + 1;
        local_24 = 1;
      }
      local_28 = 0;
      local_2c = 0;
    }
LAB_00101541:
    local_30 = local_30 + 1;
    local_18 = strtok_r(local_20," ",&local_20);
  } while( true );
}



int read_command(void **param_1,char *param_2,undefined4 *param_3)

{
  long lVar1;
  int iVar2;
  int iVar3;
  ssize_t sVar4;
  int *piVar5;
  size_t sVar6;
  void *pvVar7;
  void *pvVar8;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  
  *param_3 = 0;
  for (local_48 = 0; local_48 < 3; local_48 = local_48 + 1) {
    sprintf(param_2 + (long)local_48 * 0x40,"0");
  }
  for (local_44 = 0; local_44 < num_commands; local_44 = local_44 + 1) {
    for (local_40 = 0; local_40 < *(int *)(args_commands + (long)local_44 * 4);
        local_40 = local_40 + 1) {
      if (*(long *)((long)local_40 * 8 + *(long *)((long)local_44 * 8 + (long)*param_1)) != 0) {
        free(*(void **)((long)local_40 * 8 + *(long *)((long)local_44 * 8 + (long)*param_1)));
      }
    }
  }
  for (local_3c = 0; local_3c < num_commands; local_3c = local_3c + 1) {
    if (*(long *)((long)local_3c * 8 + (long)*param_1) != 0) {
      free(*(void **)((long)local_3c * 8 + (long)*param_1));
    }
  }
  num_commands = 0;
  sVar4 = read(0,&buff,0x1ff);
  iVar2 = (int)sVar4;
  if ((iVar2 < 0) && (piVar5 = __errno_location(), *piVar5 != 4)) {
    perror("Unable to read command. Terminating.\n");
    FUN_00101210(0xffffffff);
  }
  (&buff)[iVar2] = 0;
  sVar6 = strlen(&buff);
  if (*(char *)(sVar6 + 0x1050bf) == '\n') {
    sVar6 = strlen(&buff);
    *(undefined *)(sVar6 + 0x1050bf) = 0;
  }
  if (buff == '\0') {
    iVar2 = 0;
  }
  else {
    local_38 = tokenize_command(&buff,tokens);
    if (local_38 == 0) {
      iVar2 = 0;
    }
    else {
      if ((0 < local_38) && (iVar3 = strcmp(array + (long)(local_38 + -1) * 0x40,"&"), iVar3 == 0))
      {
        *param_3 = 1;
        array[(long)local_38 * 0x40] = 0;
        local_38 = local_38 + -1;
      }
      for (local_34 = 0; local_34 < local_38; local_34 = local_34 + 1) {
        iVar3 = strcmp(array + (long)local_34 * 0x40,"<");
        if (iVar3 == 0) {
          sprintf(array + (long)local_34 * 0x40,"%s",&DAT_00103002);
          strcpy(param_2,array + (long)(local_34 + 1) * 0x40);
          sprintf(array + (long)(local_34 + 1) * 0x40,"%s",&DAT_00103002);
        }
        iVar3 = strcmp(array + (long)local_34 * 0x40,">");
        if (iVar3 == 0) {
          sprintf(array + (long)local_34 * 0x40,"%s",&DAT_00103002);
          sprintf(param_2 + 0x40,"%s",array + (long)(local_34 + 1) * 0x40);
          sprintf(array + (long)(local_34 + 1) * 0x40,"%s",&DAT_00103002);
        }
        iVar3 = strcmp(array + (long)local_34 * 0x40,"!>");
        if (iVar3 == 0) {
          sprintf(array + (long)local_34 * 0x40,"%s",&DAT_00103002);
          sprintf(param_2 + 0x80,"%s",array + (long)(local_34 + 1) * 0x40);
          sprintf(array + (long)(local_34 + 1) * 0x40,"%s",&DAT_00103002);
        }
        iVar3 = strcmp(array + (long)local_34 * 0x40,"|");
        if (iVar3 == 0) {
          sprintf(array + (long)local_34 * 0x40,"%s",&DAT_00103002);
        }
      }
      pvVar7 = malloc((long)(iVar2 + 1) << 3);
      *param_1 = pvVar7;
      for (local_30 = 0; local_30 <= num_commands; local_30 = local_30 + 1) {
        pvVar7 = *param_1;
        pvVar8 = malloc((long)(*(int *)(args_commands + (long)local_30 * 4) + 1) << 3);
        *(void **)((long)pvVar7 + (long)local_30 * 8) = pvVar8;
        for (local_2c = 0; local_2c < *(int *)(args_commands + (long)local_30 * 4);
            local_2c = local_2c + 1) {
          lVar1 = *(long *)((long)local_30 * 8 + (long)*param_1);
          pvVar7 = malloc((long)(*(int *)(args_sizes + (long)local_30 * 4) + 2));
          *(void **)(lVar1 + (long)local_2c * 8) = pvVar7;
        }
      }
      local_28 = 0;
      for (local_24 = 0; iVar2 = num_commands, local_24 <= num_commands; local_24 = local_24 + 1) {
        for (local_20 = 0; local_20 < *(int *)(args_commands + (long)local_24 * 4);
            local_20 = local_20 + 1) {
          if (local_20 == *(int *)(args_commands + (long)local_24 * 4) + -1) {
            *(undefined8 *)((long)local_20 * 8 + *(long *)((long)local_24 * 8 + (long)*param_1)) = 0
            ;
          }
          else {
            strcpy(*(char **)((long)local_20 * 8 + *(long *)((long)local_24 * 8 + (long)*param_1)),
                   array + (long)local_28 * 0x40);
          }
          local_28 = local_28 + 1;
        }
      }
    }
  }
  return iVar2;
}




void _fini(void)

{
  return;
}


