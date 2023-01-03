/* yams -- Yet Another Machine Simulator
   Copyright (C) 2002 Juha Aatrokoski, Timo Lilja, Leena Salmela,
   Teemu Takanen, Aleksi Virtanen

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.

   $Id: disasm.c,v 1.5 2005/06/05 15:00:19 jaatroko Exp $
*/

#if 1 // Nick
#include <fcntl.h>
#include <unistd.h>
#endif

#include "cpu.h"
#include "opcodes.h"
#include "cpuregs.h"
#include "disasm.h"

char * op_codes[] = {
    "SPECIAL",
    "REGIMM",
    "J",
    "JAL",
    "BEQ",
    "BNE",
    "BLEZ",
    "BGTZ",
    "ADDI",
    "ADDIU",
    "SLTI",
    "SLTIU",
    "ANDI",
    "ORI",
    "XORI",
    "LUI",
    "COP0",
    "COP1",
    "COP2",
    "COP3",
    "BEQL",
    "BNEL",
    "BLEZL",
    "BGTZL",
    "Invalid instr",
    "Invalid instr",
    "Invalid instr",
    "Invalid instr",
    "SPECIAL2",
    "Invalid instr",
    "Invalid instr",
    "Invalid instr",
    "LB",
    "LH",
    "LWL",
    "LW",
    "LBU",
    "LHU",
    "LWR",
    "Invalid instr",
    "SB",
    "SH",
    "SWL",
    "SW",
    "Invalid instr",
    "Invalid instr",
    "SWR",
    "CACHE",
    "LL",
    "LWC1",
    "LWC2",
    "PREF",
    "Invalid instr",
    "LDC1",
    "LDC2",
    "Invalid instr",
    "SC",
    "SWC1",
    "SWC2",
    "Invalid instr",
    "Invalid instr",
    "SDC1",
    "SDC2",
    "Invalid instr",
    NULL
};

char *special_op_codes[] = {
    "SLL",
    "MOVC1",
    "SRL",
    "SRA",
    "SLLV",
    "Invalid special opcode",
    "SRLV",
    "SRAV",
    "JR",
    "JALR",
    "MOVZ",
    "MOVN",
    "SYSCALL",
    "BREAK",
    "Invalid special opcode",
    "SYNC",
    "MFHI",
    "MTHI",
    "MFLO",
    "MTLO",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "MULT",
    "MULTU",
    "DIV",
    "DIVU",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "ADD",
    "ADDU",
    "SUB",
    "SUBU",
    "AND",
    "OR",
    "XOR",
    "NOR",
    "Invalid special opcode",
    "Invalid special opcode",
    "SLT",
    "SLTU",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "TGE",
    "TGEU",
    "TLT",
    "TLTU",
    "TEQ",
    "Invalid special opcode",
    "TNE",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    "Invalid special opcode",
    NULL
};

char *special2_op_codes[] = {
    "MADD",
    "MADDU",
    "MUL",
    NULL,                         /* Invalid SPECIAL2 instruction */
    "MSUB",
    "MSUBU",
    NULL, NULL, NULL, NULL, NULL, /* Invalid SPECIAL2 instructions */
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL,
    "CLZ",
    "CLO",
};

char *regimm_codes[] = {
    "BLTZ",        /*    0x00  */
    "BGEZ",        /*    0x01  */
    "BLTZL",       /*    0x02  */
    "BGEZL",       /*    0x03  */
    NULL,
    NULL,
    NULL,
    NULL,
    "TGEI",        /*    0x08  */
    "TGEIU",       /*    0x09  */
    "TLTI",        /*    0x0a  */
    "TLTIU",       /*    0x0b  */
    "TEQI",        /*    0x0c  */
    NULL,
    "TNEI",        /*    0x0e  */
    NULL,
    "BLTZAL",      /*    0x10  */
    "BGEZAL",      /*    0x11  */
    "BLTZALL",     /*    0x12  */
    "BGEZALL",     /*    0x13  */
    NULL
};

char *cop0_rs_codes[] = {
    "MFC0",
    NULL,
    NULL,
    NULL,
    "MTC0",
    NULL
};

char *cop0_function_codes[] = {
    NULL,
    "TLBR",
    "TLBWI",
    NULL, NULL, NULL,
    "TLBWR",
    NULL,
    "TLBP",
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL,
    "ERET",
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL,
    "WAIT",
};



void disasm(uint32_t addr, instr_t *instr, char *buf, int len) {
    if(instr->instr == 0) {
    	snprintf(buf, len, "NOP");
    }
    else {
	switch(instr->opcode) {
	case OP_SPECIAL:
	    switch(instr->function) {
	    case SPEC_ADD:
	    case SPEC_ADDU:
	    case SPEC_AND:
	    case SPEC_SUB:
	    case SPEC_SUBU:
	    case SPEC_MOVZ:
	    case SPEC_MOVN:
	    case SPEC_OR:
	    case SPEC_XOR:
	    case SPEC_NOR:
	    case SPEC_SLT:
	    case SPEC_SLTU:
		snprintf(buf, len, "%s\t$%s, $%s, $%s", 
			 special_op_codes[instr->function],
			 cpu_register_names[instr->rd],
			 cpu_register_names[instr->rs],
			 cpu_register_names[instr->rt]);
		break;
	    case SPEC_SLL:
	    case SPEC_SRL:
	    case SPEC_SRA:
		snprintf(buf, len, "%s\t$%s, $%s, 0x%.4" PRIx8, 
			 special_op_codes[instr->function],
			 cpu_register_names[instr->rd],
			 cpu_register_names[instr->rt],
			 instr->sa);
		break;
	    case SPEC_SLLV:
	    case SPEC_SRLV:
	    case SPEC_SRAV:
		snprintf(buf, len, "%s\t$%s, $%s, $%s", 
			 special_op_codes[instr->function],
			 cpu_register_names[instr->rd],
			 cpu_register_names[instr->rt],
			 cpu_register_names[instr->rs]);
		break;
	    case SPEC_JR:
	    case SPEC_MTHI:
	    case SPEC_MTLO:
		snprintf(buf, len, "%s\t$%s", 
			 special_op_codes[instr->function],
			 cpu_register_names[instr->rs]);
		break;
	    case SPEC_JALR:
		snprintf(buf, len, "%s\t$%s, $%s", 
			 special_op_codes[instr->function],
			 cpu_register_names[instr->rd],			
			 cpu_register_names[instr->rs]);
		break;
	    case SPEC_MFHI:
	    case SPEC_MFLO:
		snprintf(buf, len, "%s\t$%s", 
			 special_op_codes[instr->function],
			 cpu_register_names[instr->rd]);
		break;
	    case SPEC_MULT:
	    case SPEC_MULTU:
	    case SPEC_DIV:
	    case SPEC_DIVU:
	    case SPEC_TGE:
	    case SPEC_TGEU:
	    case SPEC_TLT:
	    case SPEC_TLTU:
	    case SPEC_TEQ:
	    case SPEC_TNE:
		snprintf(buf, len, "%s\t$%s, $%s", 
			 special_op_codes[instr->function],
			 cpu_register_names[instr->rs],			
			 cpu_register_names[instr->rt]);
		break;
	    case SPEC_MOVC1:
	    case SPEC_SYSCALL:
	    case SPEC_BREAK:
	    case SPEC_SYNC:
		snprintf(buf, len, "%s", 
			 special_op_codes[instr->function]);
		break;
	    default:
		snprintf(buf, len, "Invalid instr (SPEC)");
	    }
	    break;
	case OP_REGIMM:
	    switch(instr->rt) {
	    case REGIMM_BLTZ:
	    case REGIMM_BGEZ:
	    case REGIMM_BLTZL:
	    case REGIMM_BGEZL:
	    case REGIMM_BLTZAL:
	    case REGIMM_BGEZAL:
	    case REGIMM_BLTZALL:
	    case REGIMM_BGEZALL:
		{
		    uint32_t target = addr + 4 
			+ (I_SIGN_EXTEND_16(instr->immediate) << 2);
		    
		    snprintf(buf, len, "%s\t$%s, 0x%.8" PRIx32, 
			     regimm_codes[instr->rt],
			     cpu_register_names[instr->rs],
			     target); 	       
		}
		break;
	    case REGIMM_TEQI:
	    case REGIMM_TGEI:
	    case REGIMM_TGEIU:
	    case REGIMM_TLTI:
	    case REGIMM_TLTIU:
	    case REGIMM_TNEI:
		snprintf(buf, len, "%s\t$%s, 0x%.4" PRIx32, 
			 regimm_codes[instr->rt],
			 cpu_register_names[instr->rs],
			 U_SIGN_EXTEND_16(instr->immediate)); 
		break;
	    default:
		snprintf(buf, len, "Invalid instr (REGIMM)");
		break;
	    }
	    break;
	case OP_SPECIAL2:
	    switch(instr->function) {
	    case SPECIAL2_MADD:
	    case SPECIAL2_MADDU:
	    case SPECIAL2_MSUB:
	    case SPECIAL2_MSUBU:
		snprintf(buf, len, "%s\t$%s, $%s", 
			 special2_op_codes[instr->function],
			 cpu_register_names[instr->rs],			
			 cpu_register_names[instr->rt]);
		break;
	    case SPECIAL2_MUL:
		snprintf(buf, len, "%s\t$%s, $%s, $%s", 
			 special2_op_codes[instr->function],
			 cpu_register_names[instr->rd],
			 cpu_register_names[instr->rs],
			 cpu_register_names[instr->rt]);
		break;
	    case SPECIAL2_CLZ:
	    case SPECIAL2_CLO:
		snprintf(buf, len, "%s\t$%s, $%s", 
			 special2_op_codes[instr->function],
			 cpu_register_names[instr->rd],			
			 cpu_register_names[instr->rs]);
		break;
	    default:
		snprintf(buf, len, "Invalid instr (SPEC2)");
		break;
	    }
            break;
	case OP_COP0:
	    if((instr->rs & 0x10) != 0x10) {
		switch(instr->rs) {
		case COP0_MFC0:
		case COP0_MTC0:
		    snprintf(buf, len, "%s\t$%s, $%s, 0x%.4" PRIx8, 
			     cop0_rs_codes[instr->rs],
			     cpu_register_names[instr->rt],
			     cp0_register_names[instr->rd],
			     (instr->function & 0x7));
		    break;
		default:
		    snprintf(buf, len, "Invalid instr (COP0)");
		}
	    }
	    else {
		switch(instr->function) {
		case COP0_TLBR:
		case COP0_TLBWI:
		case COP0_TLBWR:
		case COP0_TLBP:
		case COP0_ERET:
		case COP0_WAIT:
		    snprintf(buf, len, "%s",
			     cop0_function_codes[instr->function]);
		    break;
		default:
		    snprintf(buf, len, "Invalid instr (COP0)");
		}
	    }
	    break;
	case OP_ADDI:
	case OP_ADDIU:
	case OP_ANDI:
	case OP_SLTI:
	case OP_SLTIU:
	case OP_ORI:
	case OP_XORI:
	    snprintf(buf, len, "%s\t$%s, $%s, 0x%.4" PRIx16, 
		     op_codes[instr->opcode],
		     cpu_register_names[instr->rt],
		     cpu_register_names[instr->rs],
		     instr->immediate);
	    break;
	case OP_BEQ:
	case OP_BNE:
	case OP_BEQL:
	case OP_BNEL:
	    {
		uint32_t target = addr + 4 
		    + (I_SIGN_EXTEND_16(instr->immediate) << 2);
		
		snprintf(buf, len, "%s\t$%s, $%s, 0x%.8" PRIx32, 
			 op_codes[instr->opcode],
			 cpu_register_names[instr->rs],
			 cpu_register_names[instr->rt],
			 target);
	    } 
	    break;	       
	case OP_BGTZ:
	case OP_BGTZL:
	case OP_BLEZ:
	case OP_BLEZL:
	    {
		uint32_t target = addr + 4 
		    + (I_SIGN_EXTEND_16(instr->immediate) << 2);
	    
		snprintf(buf, len, "%s\t$%s, $%s, 0x%.8" PRIx32, 
			 op_codes[instr->opcode],
			 cpu_register_names[instr->rs],
			 cpu_register_names[instr->rt],
			 target); 
	    }
	    break;
	case OP_J:
	case OP_JAL:
	    {
		uint32_t target = ((addr + 4) & 0xf0000000) 
		    + (I_SIGN_EXTEND_16(instr->immediate) << 2);
	    
		snprintf(buf, len, "%s\t0x%.8" PRIx32, 
			 op_codes[instr->opcode],
			 target); 
	    }
	    break;
	case OP_LUI:
	    snprintf(buf, len, "%s\t$%s, 0x%.4" PRIx16, 
		     op_codes[instr->opcode],
		     cpu_register_names[instr->rt],
		     instr->immediate);
	    break;
	case OP_LB:
	case OP_LH:
	case OP_LWL:
	case OP_LW:
	case OP_LBU:
	case OP_LHU:
	case OP_LWR:
	case OP_SB:
	case OP_SH:
	case OP_SWL:
	case OP_SW:
	case OP_SWR:
	case OP_CACHE:
	case OP_LL:
	case OP_LWC1:
	case OP_LWC2:
	case OP_PREF:
	case OP_LDC1:
	case OP_LDC2:
	case OP_SC:
	case OP_SWC1:
	case OP_SWC2:
	case OP_SDC1:
	case OP_SDC2:
	    snprintf(buf, len, "%s\t$%s, 0x%.4" PRIx16 "($%s)", 
		     op_codes[instr->opcode],
		     cpu_register_names[instr->rt],
		     instr->immediate,
		     cpu_register_names[instr->rs]);
	    break;
	default:
	    snprintf(buf, len, "Invalid instr (OP)"); 
	}
    }
}

#if 1 // Nick
char *cpu_register_names [] = {
    "zero",
    "at",
    "v0",
    "v1",
    "a0",
    "a1",
    "a2",
    "a3",
    "t0",
    "t1",
    "t2",
    "t3",
    "t4",
    "t5",
    "t6",
    "t7",
    "s0",
    "s1",
    "s2",
    "s3",
    "s4",
    "s5",
    "s6",
    "s7",
    "t8",
    "t9",
    "k0",
    "k1",
    "gp",
    "sp",
    "fp",
    "ra",
    "pc",
    "hi",
    "lo",
    NULL
};

char *cp0_register_names[] = {
    "Index",
    "Random",
    "EntLo0",
    "EntLo1",
    "Contxt",
    "PgMask",
    "Wired",
    "CP0R7",
    "BadVAd",
    "Count",
    "EntrHi",
    "Compar",
    "Status",
    "Cause",
    "EPC",
    "PRId",
    "Conf0",
    "LLAddr",
    "WtchLo",
    "WtchHi",
    "CP020",
    "CP021",
    "Conf1",
    "Debug",
    "DEPC",
    "PrfCnt",
    "ErrCtl",
    "CacheE",
    "CP0Lo",
    "CP0Hi",
    "ErrEPC",
    "DESAVE",
    NULL
};

static void instr_decode(instr_t * instr) {
    instr->opcode = instr->instr >> 26;
    instr->rs = (instr->instr & 0x03e00000) >> 21;
    instr->rt = (instr->instr & 0x001f0000) >> 16;
    instr->rd = (instr->instr & 0x0000f800) >> 11;
    instr->sa = (instr->instr & 0x000007c0) >> 6;
    instr->instr_index = (instr->instr & 0x03ffffff);
    instr->immediate = (instr->instr & 0x0000ffff);
    instr->function = instr->instr & 0x0000003f;
}

static uint32_t mem[0x400000]; // 16M

int main(int argc, char **argv) {
  instr_t instr;

  if (argc >= 2) {
    // command-line argument is .bin file to disassemble
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
      perror(argv[1]);
      exit(EXIT_FAILURE);
    }

    ssize_t result = read(fd, mem, 0x1000000); // maximum of 16M
    if (result == (ssize_t)-1) {
      perror("read()");
      exit(EXIT_FAILURE);
    }
    int size = (int)result & ~3;

    close(fd);

    for (int pc = 0; pc < size; pc += 4) {
      instr.instr = mem[pc >> 2];
      instr_decode(&instr);

      char buf[0x100];
      disasm(pc, &instr, buf, 0x100);
      if (memcmp(buf, "Invalid instr", 13) == 0)
        snprintf(buf, 0x100, ".WORD\t0x%08x", instr.instr);
      printf("%08x %08x\t%s\n", pc, instr.instr, buf);
    } 
  }
  else {
    // with no command-line argument, print opcode table
    printf("opcodes\n");
    for (int i = 0; i < 0x40; ++i) {
      instr.instr = (i << 26) | (0 << 21) | (1 << 16);
      instr_decode(&instr);

      char buf[0x100];
      disasm(0, &instr, buf, 0x100);
      printf("%08x\t%s\n", instr.instr, buf);
    }
    printf("\n");

    printf("opcodes 00\n");
    for (int i = 0; i < 0x40; ++i) {
      instr.instr = (0 << 26) | (0 << 21) | (1 << 16) | (2 << 11) | (0 << 6) | i;
 
      instr_decode(&instr);

      char buf[0x100];
      disasm(0, &instr, buf, 0x100);
      printf("%08x\t%s\n", instr.instr, buf);
    }
    printf("\n");

    printf("opcodes 40\n");
    for (int i = 0; i < 0x10; ++i) {
      instr.instr = (0x10 << 26) | (i << 21) | (1 << 16);
 
      instr_decode(&instr);

      char buf[0x100];
      disasm(0, &instr, buf, 0x100);
      printf("%08x\t%s\n", instr.instr, buf);
    }
    printf("\n");

    printf("opcodes 42\n");
    for (int i = 0; i < 0x40; ++i) {
      instr.instr = (0x10 << 26) | (0x10 << 21) | (1 << 16) | (2 << 11) | (0 << 6) | i;
 
      instr_decode(&instr);

      char buf[0x100];
      disasm(0, &instr, buf, 0x100);
      printf("%08x\t%s\n", instr.instr, buf);
    }
    printf("\n");
  }

  return 0;
}
#endif
