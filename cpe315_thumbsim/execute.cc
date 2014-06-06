#include "thumbsim.hpp"
#define PC_REG 15
#define LR_REG 14
#define SP_REG 13
#define PC rf[PC_REG]
#define LR rf[LR_REG]
#define SP rf[SP_REG]

unsigned int signExtend16to32ui(short i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}

unsigned int signExtend8to32ui(char i) {
   return static_cast<unsigned int>(static_cast<int>(i));
}

ASPR flags;

void setCarryOverflow (int num1, int num2, OFType oftype) {
   switch (oftype) {
   case OF_ADD:
      if (((unsigned long long int)num1 + (unsigned long long int)num2) ==
            ((unsigned int)num1 + (unsigned int)num2)) {
         flags.C = 0;
      }
      else {
         flags.C = 1;
      }
      if (((long long int)num1 + (long long int)num2) ==
            ((int)num1 + (int)num2)) {
         flags.V = 0;
      }
      else {
         flags.V = 1;
      }
      break;
   case OF_SUB:
      if (num1 >= num2) {
         flags.C = 1;
      }
      else if (((unsigned long long int)num1 - (unsigned long long int)num2) ==
            ((unsigned int)num1 - (unsigned int)num2)) {
         flags.C = 0;
      }
      else {
         flags.C = 1;
      }
      if (((num1==0) && (num2==0)) || 
            (((long long int)num1 - (long long int)num2) ==
             ((int)num1 - (int)num2))) {
         flags.V = 0;
      }
      else {
         flags.V = 1;
      }
      break;
   case OF_SHIFT:
      // C flag unaffected for shifts by zero
      if (num2 != 0) {
         if (((unsigned long long int)num1 << (unsigned long long int)num2) ==
               ((unsigned int)num1 << (unsigned int)num2)) {
            flags.C = 0;
         }
         else {
            flags.C = 1;
         }
      }
      // Shift doesn't set overflow
      break;
   default:
      cerr << "Bad OverFlow Type encountered." << __LINE__ << __FILE__ << endl;
      exit(1);
   }
}

// You're given the code for evaluating BEQ, 
// and you'll need to fill in the rest of these.
// See Page 99 of the armv6 manual
static int checkCondition(unsigned short cond) {
   switch(cond) {
   case EQ:
      if (flags.Z) {
         return TRUE;
      }
      break;

   case NE:
      if (!flags.Z) {
         return TRUE;
      }
      break;

   case CS:
      if (flags.C) {
         return TRUE;
      }
      break;

   case CC:
      if (!flags.C) {
         return TRUE;
      }
      break;

   case MI:
      if (flags.N) {
         return TRUE;
      }
      break;

   case PL:
      if (!flags.N) {
         return TRUE;
      }
      break;

   case VS:
      if (flags.V) {
         return TRUE;
      }
      break;

   case VC:
      if (!flags.V) {
         return TRUE;
      }
      break;

   case HI:
      if (flags.C && !flags.Z) {
         return TRUE;
      }
      break;

   case LS:
      if (!flags.C || flags.Z) {
         return TRUE;
      }
      break;

   case GE:
      if (flags.N == flags.V) {
         return TRUE;
      }
      break;

   case LT:
      if (flags.N != flags.V) {
         return TRUE;
      }
      break;

   case GT:
      if (!flags.Z && flags.N == flags.V) {
         return TRUE;
      }
      break;

   case LE:
      if (flags.Z || flags.N != flags.V) {
         return TRUE;
      }
      break;

   case AL:
      return TRUE;
      break;
   }
   return FALSE;
}

void execute() {
   Data16 instr = imem[PC];
   Thumb_Types itype;
   unsigned int pctarget = PC + 2;
   unsigned int addri, ndx, addr;
   int diff, BitCount, bit, result;
   Data32 temp(0);
   
   /* Convert instruction to correct type */
   ALU_Type alu(instr);
   SP_Type sp(instr);
   DP_Type dp(instr);
   LD_ST_Type ld_st(instr);
   MISC_Type misc(instr);
   COND_Type cond(instr);
   UNCOND_Type uncond(instr);
   LDM_Type ldm(instr);
   STM_Type stm(instr);
   LDRL_Type ldrl(instr);
   ADD_SP_Type addsp(instr);

   ALU_Ops add_ops;
   DP_Ops dp_ops;
   SP_Ops sp_ops;
   LD_ST_Ops ldst_ops;
   MISC_Ops misc_ops;

   rf.write(PC_REG, pctarget);
   stats.instr++;

   itype = decode(ALL_Types(instr));
   switch(itype) {
   case ALU:
      add_ops = decode(alu);
      switch(add_ops) {
      case ALU_LSLI:
         rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] << alu.instr.lsli.imm);
         stats.numRegReads++;
         stats.numRegWrites++;
         break;
      case ALU_LSRI:
         rf.write(alu.instr.lsri.rd, rf[alu.instr.lsri.rm] << alu.instr.lsri.imm);
         stats.numRegReads++;
         stats.numRegWrites++;
         break;
      case ALU_ASRI:
         rf.write(alu.instr.asri.rd, rf[alu.instr.asri.rm] << alu.instr.asri.imm);
         stats.numRegReads++;
         stats.numRegWrites++;
         break;
      case ALU_ADDR:
         rf.write(alu.instr.addr.rd, rf[alu.instr.addr.rn] + rf[alu.instr.addr.rm]);
         stats.numRegReads += 2;
         stats.numRegWrites++;
         break;
      case ALU_SUBR:
         rf.write(alu.instr.subr.rd, rf[alu.instr.subr.rn] - rf[alu.instr.subr.rm]);
         stats.numRegReads += 2;
         stats.numRegWrites++;
         break;
      case ALU_ADD3I:
         rf.write(alu.instr.add3i.rd, rf[alu.instr.add3i.rn] + alu.instr.add3i.imm);
         stats.numRegReads++;
         stats.numRegWrites++;
         break;
      case ALU_SUB3I:
         rf.write(alu.instr.sub3i.rd, rf[alu.instr.sub3i.rn] - alu.instr.sub3i.imm);
         stats.numRegReads++;
         stats.numRegWrites++;
         break;
      case ALU_MOV:
         rf.write(alu.instr.mov.rdn, alu.instr.mov.imm);
         stats.numRegWrites++;
         break;
      case ALU_CMP:
         flags.N = (int)(rf[alu.instr.cmp.rdn]) - (int)(alu.instr.cmp.imm) < 0;
         flags.Z = !((int)(rf[alu.instr.cmp.rdn]) - (int)(alu.instr.cmp.imm));
         setCarryOverflow(rf[alu.instr.cmp.rdn], alu.instr.cmp.imm, OF_SUB);
         stats.numRegReads++;
         break;
      case ALU_ADD8I:
         rf.write(alu.instr.add8i.rdn, rf[alu.instr.add8i.rdn] + alu.instr.add8i.imm);
         stats.numRegReads++;
         stats.numRegWrites++;
         break;
      case ALU_SUB8I:
         rf.write(alu.instr.sub8i.rdn, rf[alu.instr.sub8i.rdn] + alu.instr.sub8i.imm);
         stats.numRegReads++;
         stats.numRegWrites++;
         break;
      default:
         break;
      }
      break;
   case DP:
      dp_ops = decode(dp);
      switch(dp_ops) {
      case DP_RSB:
         rf.write(dp.instr.DP_Instr.rdn, -rf[dp.instr.DP_Instr.rm]);
         flags.N = rf[dp.instr.DP_Instr.rdn] < 0;
         flags.Z = !rf[dp.instr.DP_Instr.rdn];
         setCarryOverflow(0, rf[dp.instr.DP_Instr.rm], OF_SUB);
         stats.numRegReads += 2;
         stats.numRegWrites++;
         break;

      case DP_ADC:
         result = rf[dp.instr.DP_Instr.rdn];
         rf.write(dp.instr.DP_Instr.rdn, rf[dp.instr.DP_Instr.rdn] + rf[dp.instr.DP_Instr.rm] + flags.C);
         flags.N = rf[dp.instr.DP_Instr.rdn] < 0;
         flags.Z = !rf[dp.instr.DP_Instr.rdn];
         setCarryOverflow(result, rf[dp.instr.DP_Instr.rm], OF_ADD);
         stats.numRegReads += 2;
         stats.numRegWrites++;
         break;

      case DP_ORR:
         rf.write(dp.instr.DP_Instr.rdn, rf[dp.instr.DP_Instr.rdn] | rf[dp.instr.DP_Instr.rm]);
         flags.N = rf[dp.instr.DP_Instr.rdn] < 0;
         flags.Z = !(rf[dp.instr.DP_Instr.rdn]);
         stats.numRegReads += 2;
         stats.numRegWrites++;
         break;

      case DP_TST:
         // Apparently only runs in -O3
         flags.N = rf[dp.instr.DP_Instr.rdn] && rf[dp.instr.DP_Instr.rm] < 0;
         flags.Z = !(rf[dp.instr.DP_Instr.rdn] && rf[dp.instr.DP_Instr.rm]);
         break;

      default:
         break;
      }
      break;
   case SPECIAL:
      sp_ops = decode(sp);
      switch(sp_ops) {
      case SP_MOV:
         rf.write(sp.instr.mov.rd + sp.instr.mov.d * 8, rf[sp.instr.mov.rm]);
         stats.numRegReads++;
         stats.numRegWrites++;
         break;
      case SP_CMP:
         setCarryOverflow(sp.instr.cmp.rd + sp.instr.cmp.d * 8, sp.instr.cmp.rm, OF_SUB);
         flags.N = sp.instr.cmp.rd + sp.instr.cmp.d * 8 - sp.instr.cmp.rm < 0;
         flags.Z = sp.instr.cmp.rd + sp.instr.cmp.d * 8 - sp.instr.cmp.rm == 0;
         break;
      case SP_ADD:
         rf.write(sp.instr.add.rd + sp.instr.add.d * 8, rf[sp.instr.add.rd] + rf[sp.instr.add.rm]);
         stats.numRegReads += 2;
         stats.numRegWrites++;
         break;
      }
      break;
   case LD_ST:
      // You'll want to use these load and store models
      // to implement ldrb/strb, ldm/stm and push/pop
      ldst_ops = decode(ld_st);
      switch(ldst_ops) {
      case STRR:
         break;
      case LDRR:
         break;
      case STRI:
         dmem.write(rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4, rf[ld_st.instr.ld_st_imm.rt]);
         stats.numRegReads += 2;
         stats.numMemWrites++;
         break;
      case LDRI:
         rf.write(ld_st.instr.ld_st_imm.rt, dmem[rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4]);
         stats.numRegWrites++;
         stats.numMemReads++;
         break;
      case STRBR:
         addr = rf[ld_st.instr.ld_st_reg.rn] + rf[ld_st.instr.ld_st_reg.rm];
         temp = dmem[addr];
         temp.set_data_ubyte4(0, rf[ld_st.instr.ld_st_reg.rt] & 0xff);
         dmem.write(addr, temp);
         stats.numMemWrites++;
         stats.numRegReads += 3;
         stats.numMemReads++;
         break;
      case STRBI:
         addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
         temp = dmem[addr];
         temp.set_data_ubyte4(0, rf[ld_st.instr.ld_st_imm.rt] & 0xff);
         dmem.write(addr, temp);
         stats.numMemWrites++;
         stats.numRegReads += 2;
         stats.numMemReads++;
         break;
      case LDRBR:
         addr = rf[ld_st.instr.ld_st_reg.rn] + rf[ld_st.instr.ld_st_reg.rm];
         rf.write(ld_st.instr.ld_st_reg.rt, dmem[addr] & 0xff);
         stats.numRegWrites++;
         stats.numRegReads += 2;
         stats.numMemReads++;
         break;
      case LDRBI:
         addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm;
         rf.write(ld_st.instr.ld_st_imm.rt, dmem[addr] & 0xff);
         stats.numRegWrites++;
         stats.numRegReads++;
         stats.numMemReads++;
         break;
      }
      break;
   case MISC:
      misc_ops = decode(misc);
      switch(misc_ops) {
      case MISC_PUSH:
         if (misc.instr.push.m) {
            rf.write(SP_REG, SP - 4);
            dmem.write(SP, LR);
            stats.numRegWrites++;
            stats.numMemWrites++;
         }
         for(int i = 7; i >= 0; i--) {
            if(1 << i & misc.instr.push.reg_list) {
               rf.write(SP_REG, SP - 4);
               dmem.write(SP, rf[i]);
               stats.numRegWrites++;
               stats.numRegReads++;
               stats.numMemWrites++;
            }
         }
         break;

      case MISC_POP:
         for(int i = 0; i < 8; i++) {
            if(1 << i & misc.instr.pop.reg_list) {
               rf.write(i, dmem[SP]);
               rf.write(SP_REG, SP + 4);
               stats.numRegWrites++;
               stats.numMemReads++;
            }
         }
         if (misc.instr.pop.m) {
            rf.write(PC_REG, dmem[SP]);
            rf.write(SP_REG, SP + 4);
            stats.numRegWrites += 2;
            stats.numMemReads++;
         }
         break;

      case MISC_SUB:
         rf.write(SP_REG, SP - (misc.instr.sub.imm*4));
         stats.numRegWrites++;
         break;

      case MISC_ADD:
         rf.write(SP_REG, SP + (misc.instr.add.imm*4));
         stats.numRegWrites++;
         break;
      }
      break;
   case COND:
      decode(cond);
      // Once you've completed the checkCondition function,
      // this should work for all your conditional branches.
      if (checkCondition(cond.instr.b.cond)){
         rf.write(PC_REG, PC + 2 * signExtend8to32ui(cond.instr.b.imm) + 2);
         stats.numRegWrites++;
      }
      break;
   case UNCOND:
      decode(uncond);
      rf.write(PC_REG, PC + 2 * signExtend16to32ui(uncond.instr.b.imm) + 2);
      stats.numRegWrites++;
      break;
   case LDM:
      ndx = 0;
      decode(ldm);
      addri = ldm.instr.ldm.rn;
      for (int i = 0; i < 8; ++i) {
         if (1 << i & ldm.instr.ldm.reg_list) {
            rf.write(dmem[addri + ndx++ * 4], i);
            stats.numRegWrites++;
            stats.numMemReads++;
         }
      }
      if (!(1 << ldm.instr.ldm.rn & ldm.instr.ldm.reg_list)) {
         rf.write(ldm.instr.ldm.rn, addri + ndx * 4);
         stats.numRegWrites++;
      }
      break;
   case STM:
      ndx = 0;
      decode(stm);
      for (int i = 0; i < 8; ++i) {
         if (1 << i & stm.instr.stm.reg_list) {
            dmem.write(rf[stm.instr.stm.rn] + ndx++ * 4, rf[i]);
            stats.numRegReads += 2;
            stats.numMemWrites++;
         }
      }
      rf.write(stm.instr.stm.rn, rf[stm.instr.stm.rn] - ndx * 4);
      stats.numRegWrites++;
      stats.numRegReads++;
      break;
   case LDRL:
      decode(ldrl); 
      // Need to check for alignment by 4
      if (PC & 2) {
        addr = PC + 2 + (ldrl.instr.ldrl.imm)*4;
      }
      else {
        addr = PC + (ldrl.instr.ldrl.imm)*4;
      }
      // Requires two consecutive imem locations pieced together
      temp = imem[addr] | (imem[addr+2]<<16);  // temp is a Data32
      rf.write(ldrl.instr.ldrl.rt, temp);
      
      // One write for updated reg
      stats.numRegWrites++;
      // One read of the PC
      stats.numRegReads++;
      // One mem read, even though it's imem, and there's two of them
      stats.numMemReads++;
      break;
   case ADD_SP:
      decode(addsp);
      rf.write(addsp.instr.add.rd, SP + (addsp.instr.add.imm*4));
      stats.numRegWrites++;
      break;
   default:
      cout << "[ERROR] Unknown Instruction to be executed" << endl;
      exit(1);
      break;
   }
}
