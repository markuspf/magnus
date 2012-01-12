###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
a=Magnus.PermutationRepresentation()
b=Magnus.PermutationRepresentationFromCopy(a)
###x=b.representativeOf(w1)  ###out of bounds
x=b.getRepresentatives()
x=b.isEmpty()
###b.printOn(["xxw"])   ###wrong names
#######
###test CosetEnumerator
w1=Magnus.Word()
w2=Magnus.Word()
g=Magnus.FPGroup()
a=Magnus.CosetEnumerator(g)
b=Magnus.CosetEnumeratorFromVector(g,[w1,w2])
x=b.enumerate()
###x=b.schreierRepresentatives()  ###cosets has to be enumerated first
x=b.permutationRepresentation()  ###core dumped because of return a private reference
