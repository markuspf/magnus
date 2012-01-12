###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
fg=Magnus.FreeGroup(2)
a=Magnus.SGofFreeGroup( fg, ws) 
###
a=Magnus.ProperSubgroupEnumerator(fg, ws)
###undefined b=Magnus.ProperSubgroupEnumeratorFromWord(fg, w1)
###undefined b=Magnus.ProperSubgroupEnumeratorFromProperSubgroupEnumerator(a)
x=a.getNextProperSubgroup(sg)
x=a.getFileName() 
###undefined a!=b
