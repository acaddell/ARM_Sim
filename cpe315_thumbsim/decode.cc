#include <iostream>
#include <iomanip>
#include "thumbsim.hpp"
#define PC_REG 15
#define PC rf[PC_REG]

using namespace std;

/*unsigned int signExtend16to32ui(short i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}

unsigned int signExtend8to32ui(char i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}*/

/* Decode to find what type it is */
Thumb_Types decode (const ALL_Types);

/* Decodes for each type */
ALU_Ops decode (const ALU_Type);
DP_Ops decode (const DP_Type);
SP_Ops decode (const SP_Type);
LD_ST_Ops decode (const LD_ST_Type);
MISC_Ops decode (const MISC_Type);
BL_Ops decode (const BL_Type);
int decode (const COND_Type);
int decode (const UNCOND_Type);
int decode (const LDM_Type);
int decode (const STM_Type);
int decode (const LDRL_Type);
int decode (const ADD_SP_Type);

Thumb_Types decode (const ALL_Types data) {
   if (data.type.alu.instr.class_type.type_check == ALU_TYPE) {
      return ALU;
   }
   else if (data.type.dp.instr.class_type.type_check == DP_TYPE) {
      return DP;
   }
   else if (data.type.sp.instr.class_type.type_check == SP_TYPE) {
      return SPECIAL;
   }
   else if (data.type.uncond.instr.class_type.type_check == UNCOND_TYPE) {
      return UNCOND;
   }
   else if (data.type.misc.instr.class_type.type_check == MISC_TYPE) {
      return MISC;
   }
   else if (data.type.cond.instr.class_type.type_check == COND_TYPE) {
      return COND;
   }
   else if (data.type.ldm.instr.class_type.type_check == LDM_TYPE) {
      return LDM;
   }
   else if (data.type.stm.instr.class_type.type_check == STM_TYPE) {
      return STM;
   }
   else if (data.type.ldrl.instr.class_type.type_check == LDRL_TYPE) {
      return LDRL;
   }
   else if (data.type.addsp.instr.class_type.type_check == ADD_SP_TYPE) {
      return ADD_SP;
   }
   else if (data.type.bl.instr.class_type.type_check == BL_TYPE) {
      return BL;
   }
   else {
      if (data.type.ld_st.instr.class_type.opA == LD_ST_REG_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMM_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMB_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMH_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMSP_OPA) {
      }
      else {
         cout << "NO TYPE FOUND" << endl;
         return ERROR_TYPE;
      }
      return LD_ST;
   }
}

ALU_Ops decode (const ALU_Type data) {
   if (data.instr.lsli.op == ALU_LSLI_OP) {
      if (opts.instrs) { 
         cout << dec << "lsls r" << data.instr.lsli.rd  << ", r" << data.instr.lsli.rm << ", #" << data.instr.lsli.imm << endl;
      }
      return ALU_LSLI;
   }
   else if (data.instr.lsri.op == ALU_LSRI_OP) {
      if (opts.instrs) { 
         cout << dec << "lsrs r" << data.instr.lsli.rd  << ", r" << data.instr.lsli.rm << ", #" << data.instr.lsli.imm << endl;
      }
      return ALU_LSRI;
   }
   else if (data.instr.asri.op == ALU_ASRI_OP) {
      if (opts.instrs) { 
         cout << dec << "asrs r" << data.instr.lsli.rd  << ", r" << data.instr.lsli.rm << ", #" << data.instr.lsli.imm << endl;
      }
      return ALU_ASRI;
   }
   else if (data.instr.addr.op == ALU_ADDR_OP) {
      if (opts.instrs) { 
         cout << dec << "adds r" << data.instr.addr.rd  << ", r" << data.instr.addr.rn << ", r" << data.instr.addr.rm << endl;
      }
      return ALU_ADDR;
   }
   else if (data.instr.subr.op == ALU_SUBR_OP) {
      if (opts.instrs) { 
         cout << dec << "subs r" << data.instr.addr.rd  << ", r" << data.instr.addr.rn << ", r" << data.instr.addr.rm << endl;
      }
      return ALU_SUBR;
   }
   else if (data.instr.add3i.op == ALU_ADD3I_OP) {
      if (opts.instrs) { 
         cout << dec << "adds r" << data.instr.add3i.rd << ", r" << data.instr.add3i.rn << ", #" << data.instr.add3i.imm << endl;
      }
      return ALU_ADD3I;
   }
   else if (data.instr.sub3i.op == ALU_SUB3I_OP) {
      if (opts.instrs) { 
         cout << dec << "subs r" << data.instr.sub3i.rd << ", r" << data.instr.sub3i.rn << ", #" << data.instr.sub3i.imm << endl;
      }
      return ALU_SUB3I;
   }
   else if (data.instr.add8i.op == ALU_ADD8I_OP) {
      if (opts.instrs) { 
         cout << dec << "adds r" << data.instr.add8i.rdn << ", #" << data.instr.add8i.imm << endl;
      }
      return ALU_ADD8I;
   }
   else if (data.instr.sub8i.op == ALU_SUB8I_OP) {
      if (opts.instrs) { 
         cout << dec << "subs r" << data.instr.sub8i.rdn << ", #" << data.instr.sub8i.imm << endl;
      }
      return ALU_ADD8I;
   }
   else if (data.instr.cmp.op == ALU_CMP_OP) { 
      if (opts.instrs) { 
         cout << dec << "cmp r" << data.instr.cmp.rdn << ", #" << data.instr.cmp.imm << endl;
      }
      return ALU_CMP;
   }
   else if (data.instr.mov.op == ALU_MOV_OP) { 
      if (opts.instrs) { 
         cout << dec << "movs r" << data.instr.mov.rdn << ", #" << (data.instr.mov.imm) << endl;
      }
      return ALU_MOV;
   }

}

DP_Ops decode (const DP_Type data) {
   if (data.instr.DP_Instr.op == 9) {
      if (opts.instrs) { 
         cout << dec << "rsbs r" << data.instr.DP_Instr.rdn  << ", r" << data.instr.DP_Instr.rm << ", #0" << endl;
      }
      return DP_RSB;
   }
   else if (data.instr.DP_Instr.op == 5) {
      if (opts.instrs) { 
         cout << dec << "adcs r" << data.instr.DP_Instr.rdn  << ", r" << data.instr.DP_Instr.rdn << ", r" << data.instr.DP_Instr.rm << endl;
      }
      return DP_ADC;
   }
   else if (data.instr.DP_Instr.op == 12) {
      if (opts.instrs) { 
         cout << dec << "orrs r" << data.instr.DP_Instr.rdn  << ", r" << data.instr.DP_Instr.rdn << ", r" << data.instr.DP_Instr.rm << endl;
      }
      return DP_ORR;
   }
   else if (data.instr.DP_Instr.op == 8) {
      if (opts.instrs) { 
      }
      return DP_TST;
   }
   else if (data.instr.DP_Instr.op == 0) {
      if (opts.instrs) { 
      }
      return DP_AND;
   }
   else if (data.instr.DP_Instr.op == 1) {
      if (opts.instrs) { 
      }
      return DP_EOR;
   }
   else if (data.instr.DP_Instr.op == 2) {
      if (opts.instrs) { 
      }
      return DP_LSL;
   }
   else if (data.instr.DP_Instr.op == 3) {
      if (opts.instrs) { 
      }
      return DP_LSR;
   }
   else if (data.instr.DP_Instr.op == 4) {
      if (opts.instrs) { 
      }
      return DP_ASR;
   }
   else if (data.instr.DP_Instr.op == 6) {
      if (opts.instrs) { 
      }
      return DP_SBC;
   }
   else if (data.instr.DP_Instr.op == 10) {
      if (opts.instrs) { 
         cout << dec << "cmp r" << data.instr.DP_Instr.rdn << ", r" << data.instr.DP_Instr.rm << endl;
      }
      return DP_CMP;
   }
   else if (data.instr.DP_Instr.op == 11) {
      if (opts.instrs) { 
      }
      return DP_CMN;
   }
   else if (data.instr.DP_Instr.op == 13) {
      if (opts.instrs) { 
      }
      return DP_MUL;
   }
   else if (data.instr.DP_Instr.op == 14) {
      if (opts.instrs) { 
      }
      return DP_BIC;
   }
   else if (data.instr.DP_Instr.op == 15) {
      if (opts.instrs) { 
      }
      return DP_MVN;
   }
}

SP_Ops decode (const SP_Type data) {
   if (data.instr.mov.op == 2) {
      if (opts.instrs) {
         if (13 == data.instr.mov.rd + data.instr.mov.d * 8) {
            cout << dec << "mov sp, r" << data.instr.mov.rm << endl;
         }
         else if (13 == data.instr.mov.rm) {
            cout << dec << "mov r" << data.instr.mov.rd + data.instr.mov.d * 8 << ", sp" << endl;
         }
         else {
            cout << dec << "mov r" << data.instr.mov.rd + data.instr.mov.d * 8 << ", r" << data.instr.mov.rm << endl;
         }
      }
      return SP_MOV;
   }
   else if (data.instr.cmp.op == 1) {
      if (opts.instrs) {
         if (13 == data.instr.cmp.rd + data.instr.cmp.d * 8) {
            cout << dec << "cmp sp, r" << data.instr.cmp.rm << endl;
         }
         else {
            cout << dec << "cmp r" << data.instr.cmp.rd + data.instr.cmp.d * 8 << ", r" << data.instr.cmp.rm << endl;
         }
      }
      return SP_CMP;
   }
   else if (data.instr.add.op == 0) {
      if (opts.instrs) {
         if (13 == data.instr.add.rd + data.instr.add.d * 8) {
            cout << dec << "add sp, r" << data.instr.add.rm << endl;
         }
         else {
            cout << dec << "add r" << data.instr.add.rd + data.instr.add.d * 8 << ", r" << data.instr.add.rm << endl;
         }
      }
      return SP_ADD;
   }
   else {
      if (opts.instrs) {
         cout << "nop" << endl;
      }
   }

}
LD_ST_Ops decode (const LD_ST_Type data) {
   if (data.instr.class_type.opA == LD_ST_REG_OPA) {
      // might need to print ld/st reg instructions
      if (data.instr.class_type.opB == LD_ST_OPB_STR) {
         return STRR;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_LDR) {
         return LDRR;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_STRB) {
         return STRBR;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_LDRB) {
         return LDRBR;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_STRH) {
         return STRHR;
      }
      else if (data.instr.class_type.opB == LD_ST_OPB_LDRH) {
         return LDRHR;
      }
   }
   else if (data.instr.class_type.opA == LD_ST_IMM_OPA) {
      if (data.instr.class_type.opB >> 2 == LD_ST_ST) {
         if (opts.instrs) { 
            cout << dec << "str r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
         }
         return STRI;
      }
      else if (data.instr.class_type.opB >> 2 == LD_ST_LD) {
         if (opts.instrs) { 
            cout << dec << "ldr r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
         }
         return LDRI;
      }
   }
   else if (data.instr.class_type.opA == LD_ST_IMMB_OPA) {
      if ((data.instr.class_type.opB >> 2) == LD_ST_ST) {
         if (opts.instrs) { 
            cout << dec << "strb r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm) << "]" << endl;
         }
         return STRBI;
      }
      else if ((data.instr.class_type.opB >> 2) == LD_ST_LD) {
         if (opts.instrs) { 
            cout << dec << "ldrb r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm) << "]" << endl;
         }
         return LDRBI;
      }
   }
   else if (data.instr.class_type.opA == LD_ST_IMMH_OPA) {
      if ((data.instr.class_type.opB >> 2) == LD_ST_ST) {
         if (opts.instrs) { 
            cout << dec << "strh r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*2) << "]" << endl;
         }
         return STRHI;
      }
      else if ((data.instr.class_type.opB >> 2) == LD_ST_LD) {
         if (opts.instrs) { 
            cout << dec << "ldrh r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*2) << "]" << endl;
         }
         return LDRHI;
      }
   }
   else if (data.instr.class_type.opA == LD_ST_IMMSP_OPA) {
      if ((data.instr.class_type.opB >> 2) == LD_ST_ST) {
         if (opts.instrs) { 
            cout << dec << "str r" << data.instr.ld_st_imm.rt << ", [SP, #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
         }
         return STRSP;
      }
      else if ((data.instr.class_type.opB >> 2) == LD_ST_LD) {
         if (opts.instrs) { 
            cout << dec << "ldr r" << data.instr.ld_st_imm.rt << ", [SP, #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
         }
         return LDRSP;
      }
   }
}

MISC_Ops decode (const MISC_Type data) {
   if (data.instr.push.op == MISC_PUSH_OP) {
      if (opts.instrs) { 
         bool multiple = FALSE;
         cout << dec << "push ";
         cout << "{";
         if (data.instr.push.reg_list & 1) {
            cout << "r0";
            multiple = TRUE;
         }
         if (data.instr.push.reg_list & 2) {
            if (multiple)
               cout << ", ";
            cout << "r1";
            multiple = TRUE;
         }
         if (data.instr.push.reg_list & 4) {
            if (multiple)
               cout << ", ";
            cout << "r2";
            multiple = TRUE;
         }
         if (data.instr.push.reg_list & 8) {
            if (multiple)
               cout << ", ";
            cout << "r3";
            multiple = TRUE;
         }
         if (data.instr.push.reg_list & 16) {
            if (multiple)
               cout << ", ";
            cout << "r4";
            multiple = TRUE;
         }
         if (data.instr.push.reg_list & 32) {
            if (multiple)
               cout << ", ";
            cout << "r5";
            multiple = TRUE;
         }
         if (data.instr.push.reg_list & 64) {
            if (multiple)
               cout << ", ";
            cout << "r6";
            multiple = TRUE;
         }
         if (data.instr.push.reg_list & 128) {
            if (multiple)
               cout << ", ";
            cout << "r7";
            multiple = TRUE;
         }
         if (data.instr.push.m) {
            if (multiple)
               cout << ", ";
            cout << "lr";
         }
         cout << "}" << endl;
      }
      return MISC_PUSH;
   }
   else if (data.instr.pop.op == MISC_POP_OP) {
      if (opts.instrs) { 
         bool multiple = FALSE;
         cout << dec << "pop ";
         cout << "{";
         if (data.instr.pop.reg_list & 1) {
            cout << "r0";
            multiple = TRUE;
         }
         if (data.instr.pop.reg_list & 2) {
            if (multiple)
               cout << ", ";
            cout << "r1";
            multiple = TRUE;
         }
         if (data.instr.pop.reg_list & 4) {
            if (multiple)
               cout << ", ";
            cout << "r2";
            multiple = TRUE;
         }
         if (data.instr.pop.reg_list & 8) {
            if (multiple)
               cout << ", ";
            cout << "r3";
            multiple = TRUE;
         }
         if (data.instr.pop.reg_list & 16) {
            if (multiple)
               cout << ", ";
            cout << "r4";
            multiple = TRUE;
         }
         if (data.instr.pop.reg_list & 32) {
            if (multiple)
               cout << ", ";
            cout << "r5";
            multiple = TRUE;
         }
         if (data.instr.pop.reg_list & 64) {
            if (multiple)
               cout << ", ";
            cout << "r6";
            multiple = TRUE;
         }
         if (data.instr.pop.reg_list & 128) {
            if (multiple)
               cout << ", ";
            cout << "r7";
            multiple = TRUE;
         }
         if (data.instr.pop.m) {
            if (multiple)
               cout << ", ";
            cout << "pc";
         }
         cout << "}" << endl;
      }
      return MISC_POP;
   }
   else if (data.instr.sub.op == MISC_SUB_OP) {
      if (opts.instrs) { 
         cout << dec << "sub sp, #";
         cout << setbase(10) << (static_cast<unsigned int>(data.instr.sub.imm)<< 2) << endl;
      } 
      return MISC_SUB;
   }
   else if (data.instr.add.op == MISC_ADD_OP) {
      if (opts.instrs) { 
         cout << "add sp, #";
         cout << setbase(10) << (static_cast<unsigned int>(data.instr.add.imm)<< 2) << endl;
      } 
      return MISC_ADD;
   }
   else if (data.instr.class_type.type_check == ADD_SP8I_TYPE) {
      if (opts.instrs) { 
         cout << "add sp, #" << setbase(10) << (data.instr.add.imm<<2)<< endl;
      }
      return MISC_ADD;
   }

}

int decode (const COND_Type data) {
   if (opts.instrs) { 
      cout << "b";
      printCond(data.instr.b.cond);
      //cout << " 0x" << hex << data.instr.b.imm << endl;
      cout << " 0x" << hex << rf[15] + 2 * static_cast<unsigned int>(static_cast<int>((char)(data.instr.b.imm))) + 2 << endl;
   }
}

int decode (const UNCOND_Type data) {
   if (opts.instrs) { 
      //cout << "b 0x" << hex << data.instr.b.imm << endl;
      cout << "b 0x" << hex << rf[15] + 2 * static_cast<unsigned int>(static_cast<int>(data.instr.b.imm)) + 2 << endl;
   }
}

int decode (const LDM_Type data) {
   if (opts.instrs) {
      cout << dec << "ldm r" << data.instr.ldm.rn;
      if (data.instr.ldm.reg_list & 1 << data.instr.ldm.rn) {
         cout << "!";
      }
      cout << ", {";
      for (int i = 0, first = 1; i < 8; ++i) {
         if (data.instr.ldm.reg_list & 1 << i) {
            if (first) {
               cout << "r" << i;
            }
            else {
               cout << ", r" << i;
            }
         }
      }
      cout << "}" << endl;
   }
   //cout << "LDM_TYPE" << endl;
   return LDM;
}

int decode (const STM_Type data) {
   if (opts.instrs) {
      cout << dec << "stm r" << data.instr.stm.rn;
      if (data.instr.stm.reg_list & 1 << data.instr.stm.rn) {
         cout << "!";
      }
      cout << ", {";
      for (int i = 0, first = 1; i < 8; ++i) {
         if (data.instr.stm.reg_list & 1 << i) {
            if (first) {
               cout << "r" << i;
            }
            else {
               cout << ", r" << i;
            }
         }
      }
      cout << "}" << endl;
   }
   //cout << "STM_TYPE" << endl;
   return STM;
}

int decode (const LDRL_Type data) {
   if (opts.instrs) { 
      cout << "ldr " << data.instr.ldrl.rt << ", 0x" << hex << data.instr.ldrl.imm << endl;
   }
   return LDRL;
}

int decode (const ADD_SP_Type data) {
   if (opts.instrs) { 
      cout << dec << "add r" << data.instr.add.rd << ", sp, #" << data.instr.add.imm << endl;
   }
   return ADD_SP;
}

BL_Ops decode (const BL_Type data) {
   if (opts.instrs) { 
      //cout << "bl 0x" << hex << data.instr.bl_upper.imm10 << endl;
      cout << "bl 0x" << hex << rf[15] + static_cast<unsigned int>(static_cast<int>(data.instr.bl_upper.imm10)) << endl;
   }
   return BL_UPPER;
}
