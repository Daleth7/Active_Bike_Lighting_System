[Transient Analysis]
{
   Npanes: 4
   {
      traces: 4 {524290,0,"V(chrg_state)"} {589830,0,"V(chrg)"} {34668548,1,"I(Charger)"} {34603019,1,"I(D1)"}
      X: (' ',1,0,0.3,3)
      Y[0]: (' ',1,-0.7,0.7,9.1)
      Y[1]: (' ',0,-14,2,14)
      Volts: (' ',0,0,0,-0.7,0.7,9.1)
      Amps: (' ',0,0,0,-14,2,14)
      Log: 0 0 0
      GridStyle: 1
   },
   {
      traces: 2 {524291,0,"V(pack_bot)"} {524298,0,"V(pack_top)"}
      X: (' ',1,0,0.3,3)
      Y[0]: (' ',1,3.2,0.4,7.2)
      Y[1]: ('_',0,1e+308,0,-1e+308)
      Volts: (' ',0,0,1,3.2,0.4,7.2)
      Log: 0 0 0
      GridStyle: 1
   },
   {
      traces: 1 {524293,0,"V(out)"}
      X: (' ',1,0,0.3,3)
      Y[0]: (' ',1,0,0.7,7.7)
      Y[1]: ('_',0,1e+308,0,-1e+308)
      Volts: (' ',0,0,0,0,0.7,7.7)
      Log: 0 0 0
      GridStyle: 1
   },
   {
      traces: 4 {34603020,0,"I(V1)"} {34603015,0,"I(V2)"} {34603016,0,"I(V3)"} {34603017,0,"I(V4)"}
      X: (' ',1,0,0.3,3)
      Y[0]: (' ',0,-10,1,2)
      Y[1]: (' ',1,1e+308,0.9,-1e+308)
      Amps: (' ',0,0,0,-10,1,2)
      Log: 0 0 0
      GridStyle: 1
   }
}
