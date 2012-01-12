# LaTeX2HTML 2K.1beta (1.47)
# Associate images original text with physical files.


$key = q/vphantom{xxx};MSF=1.6;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="4" HEIGHT="14" ALIGN="BOTTOM" BORDER="0"
 SRC="|."$dir".q|img1.gif"
 ALT="$ \vphantom{xxx}$">|; 

$key = q/n=30;MSF=1.6;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="51" HEIGHT="14" ALIGN="BOTTOM" BORDER="0"
 SRC="|."$dir".q|img2.gif"
 ALT="$ n = 30$">|; 

$key = q/n<30;MSF=1.6;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="51" HEIGHT="28" ALIGN="MIDDLE" BORDER="0"
 SRC="|."$dir".q|img4.gif"
 ALT="$ n &lt; 30$">|; 

$key = q/nge30;MSF=1.6;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="51" HEIGHT="28" ALIGN="MIDDLE" BORDER="0"
 SRC="|."$dir".q|img3.gif"
 ALT="$ n \ge 30$">|; 

1;

