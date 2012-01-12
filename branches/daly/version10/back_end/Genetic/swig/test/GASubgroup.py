###this is a test file by xuxw(xuxw76@yahoo.com)
###it's all right
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
####
a=Magnus.GASubgroup( ) 
b=Magnus.GASubgroupFromSet( ws) 
x=a.generators( ) 
x=a.fitness(b) 
x=a.mutate( ) 
x=a.crossover(b ) 
x=a.randomSubgroup( ) 
###xxwundone  friend istream& operator >> ( istream& istr, GHNConfig& C )
print a