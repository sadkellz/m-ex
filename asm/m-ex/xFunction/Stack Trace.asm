#To be inserted at 80394bd4
.include "../../Globals.s"
.include "../Header.s"

backup

    xFunc_Search:
    .set REG_xFuncLookup, 29
      lwz REG_xFuncLookup,OFST_XFunctionLookup(rtoc)
    # Check if this LR address in from any xFunctions
    .set REG_LoopCount, 28
    .set REG_xFuncNum, 27
    .set REG_Curr, 26
      li REG_LoopCount, 0
      lwz REG_xFuncNum,xFuncLookup_Num(REG_xFuncLookup)
      addi REG_Curr, REG_xFuncLookup, xFuncLookup_Start                  # get to xFunction ptr array
      b xFunc_Search_LoopCheck
    xFunc_Search_Loop:
    .set REG_xFunc, 25
    # get this xFunc
      lwz REG_xFunc,0x0(REG_Curr)
    # check if LR lies within this xFunctions codeblock
      lwz r3, ftX_Code(REG_xFunc)
      lwz	r4, 0x0198 (r30)        # instruction
      cmpw r4, r3
      blt xFunc_Search_LoopInc
      lwz r5, ftX_CodeSize(REG_xFunc)
      add r3, r3, r5
      cmpw r4, r3
      bgt xFunc_Search_LoopInc
        
        Symbol_Search:
        # Now find which function we're in
        .set REG_LoopCount2, 24
        .set REG_SymbolNum, 23
          li REG_LoopCount2, 0
          lwz REG_SymbolNum, ftX_DebugSymCount (REG_xFunc)
        Symbol_Search_Loop:
        .set REG_Symbol, 12
        # get this function symbol 
          lwz r3, ftX_DebugSym (REG_xFunc)
          mulli r4, REG_LoopCount2, ftX_DebugSym_Size
          add REG_Symbol, r3, r4
        # check func start
          lwz r3, ftX_Code(REG_xFunc)
          lwz r4, ftX_DebugSym_Start (REG_Symbol)
          add r3, r3, r4
          lwz	r4, 0x0198 (r30)        # instruction
          cmpw r4,r3
          blt Symbol_Search_LoopInc
        # check func end
          lwz r3, ftX_Code(REG_xFunc)
          lwz r4, ftX_DebugSym_End (REG_Symbol)
          add r3, r3, r4
          lwz	r4, 0x0198 (r30)        # instruction
          cmpw r4,r3
          bgt Symbol_Search_LoopInc

        # get func name and output it
          lwz r4, ftX_DebugSym_FuncName (REG_Symbol)
          b Output

        Symbol_Search_LoopInc:
          addi REG_LoopCount2, REG_LoopCount2, 1
        Symbol_Search_LoopCheck:
          cmpw REG_LoopCount2, REG_SymbolNum
          blt Symbol_Search_Loop

    xFunc_Search_LoopInc:
      addi REG_LoopCount,REG_LoopCount,1
      addi REG_Curr,REG_Curr,xFuncLookup_Start_Size
    xFunc_Search_LoopCheck:
      cmpw REG_LoopCount,REG_xFuncNum
      blt xFunc_Search_Loop
    xFunc_Search_LoopEnd:

# Get null string
  bl NullString
  mflr r4

Output:
# OSReport symbol name
  bl StackInfo
  mflr r3
  branchl r12,OSReport

b Exit

StackInfo:
blrl
.string "Symbol: %s\n"
.align 2

NullString:
blrl
.string "N/A"
.align 2


Exit:
  restore
  rlwinm.	r0, r27, 0, 6, 6