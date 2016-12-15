print "\n".join(["".join([("BinBin-MyLove"[(x-y)%13]if((x*0.05)**2+(y*0.1)**2-1)**3-(x*0.05)**2*(y*0.1)**3<=0 else" ")
                         for x in range(-60,60)])for y in range(30,-30,-1)])