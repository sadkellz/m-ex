#To be inserted @ 801604b4
.include "../../Globals.s"
.include "../Header.s"

  lwz r3, OFST_ClassicTrophyLookup(rtoc)
  add r3, r3, r0
  lhz r3, 0x00(r3)
