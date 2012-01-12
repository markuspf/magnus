import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
fg=Magnus.FreeGroup(2)
###
a=Magnus.TwoCommSolver( fg)
x=a.isProductOfTwoComms(w1,w1,w1,w2,w2 )
       

