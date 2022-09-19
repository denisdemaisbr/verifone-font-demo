ddl -p1 -r config.sys ^
-c -z -x 123456 ^
RESETSYSERR=1 ^
*GO=main.out ^
*FA=0 ^
*DHEAP=1 ^
*DEBUG=0 ^
*SMDL=1 ^
*NO.VXNCP=1 *NO.VXCE=1 *NO.VXBTE=1 *NO.VXBT=1 *NO.VXWIFI=1 *NO.VXCTLS=1 *NO.VXGUI=1 ^
-i main.out ^
-i main.out.p7s

pause
