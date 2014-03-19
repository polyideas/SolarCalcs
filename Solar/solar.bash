#!/bin/bash  
PI=3.14159265

LAT=$1
LONG=$2
DATE=$3
TIME=$4
RES=$5

function debug_pr (){
	if [ "$RES" == "1"  ]; then
		echo $1
	fi
}

help_msg="solar.pl <Latitude> <Longitude> <Date (MM/DD/YYYY)> <Time HH:MM:SS> <output result>"

if [ "$LAT" == "-h" ]; then
	echo $help_msg
	exit
fi

arrTime=(`echo $TIME | tr ":" "\n"`)
hours=${arrTime[0]}
minutes=${arrTime[1]}

arrDate=(`echo $DATE | tr "/" "\n"`)
day=${arrDate[1]}
month=${arrDate[0]}
year=${arrDate[2]}



#JulDay=`./jdate.bash 20140222`
#JulDay=2456710.41666667
JulDay=`bash ./jdate.bash $year$month$day`
debug_pr $JulDay

JulDay=`echo "$JulDay-0.5+($hours*60+$minutes)/1440" | bc -l`

#JulDay=`echo "(367*$Year - INT(7*($Year + INT(($Month+9)/12))/4) - INT(3*(INT(($Year+($Month-9)/7)/100)+1)/4) + INT(275*$Month/9) + $Day + 1721028.5)+(($Minute/60)+$Hour)/24" | bc -l`

debug_pr $JulDay

JulCentury=`echo "($JulDay-2451545.0)/36525.0" | bc -l`
G2="0"$JulCentury
debug_pr $G2


GeomMeanLongSun=`echo "(280.46646+$G2*(36000.76983+$G2*0.0003032))/360" | bc` 
tmp1=`echo "(280.46646+$G2*(36000.76983+$G2*0.0003032))" | bc -l`
tmp2=`echo "$GeomMeanLongSun*360" | bc -l`
I2=`echo "$tmp1-$tmp2" | bc -l`
debug_pr "GeomMeanLongSun=$I2"

GeomMeanAnomSun=`echo "357.52911+$G2*(35999.05029-0.0001537*$G2)"| bc -l`
J2=$GeomMeanAnomSun
debug_pr "GeomMeanAnomSun=$J2"


EccentEarchOrbit=`echo "0.016708634-$G2*(0.000042037+0.0000001267*$G2)"| bc -l`
K2=$EccentEarchOrbit
debug_pr "EccentEarchOrbit=$K2"

#SunEqOfCtr
p1=`echo "s($J2*$PI/180)*(1.914602-$G2*(0.004817+0.000014*$G2))" | bc -l`
p2=`echo "s($J2*2*$PI/180)*(0.019993-0.000101*$G2)" | bc -l`
p3=`echo "s($J2*3*$PI/180)*0.000289"`
SunEqOfCtr=`echo "$p1+$p2+$p3" | bc -l`
L2=$SunEqOfCtr
debug_pr "SunEqOfCtr=$L2"

SunTrueLong=`echo "$I2+$L2" | bc -l`
M2=$SunTrueLong
debug_pr "SunTrueLong=$M2"

SunTrueAnom=`echo "$J2+$L2" | bc -l`
N2=$SunTrueAnom
debug_pr "SunTrueAnom=$N2"

SunRadVector=`echo "(1.000001018*(1-$K2*$K2))/(1+$K2*c($N2*$PI/180))" | bc -l`
O2=$SunRadVector
debug_pr "SunRadVector=$O2"


SunAppLong=`echo "$M2-0.00569-0.00478*s((125.04-1934.136*$G2)*$PI/180)" | bc -l`
P2=$SunAppLong
debug_pr "SunAppLong=$P2"

MeanObliqEcliptic=`echo "23+(26+((21.448-$G2*(46.815+$G2*(0.00059-$G2*0.001813))))/60)/60" | bc -l`
Q2=$MeanObliqEcliptic
debug_pr "MeanObliqEcliptic=$Q2"

ObliqCorr=`echo "$Q2+0.00256*c((125.04-1934.136*$G2)*$PI/180)" | bc -l`
R2=$ObliqCorr
debug_pr "ObliqCorr=$R2"


SunRtAscen=`echo "(atan2(c($R2*$PI/180),c($P2*$PI/180))*s($P2*$PI/180))*180/$PI" | bc -l atan2.bc`
S2=$SunRtAscen
debug_pr "SunRtAscen=$S2"


SunDeclin=`echo "(asin(s($R2*$PI/180)*s($P2*$PI/180)))*180/$PI" | bc -l arcsin.bc`
T2=$SunDeclin
debug_pr "SunDeclin=$T2"

varY=`echo "tan(deg2rad($R2/2))*tan(deg2rad($R2/2))" | bc -l arcsin.bc`
U2=$varY
debug_pr "varY=$U2"

EqOfTime=`echo "4*rad2deg($U2*s(2*deg2rad($I2))-2*$K2*s(deg2rad($J2))+4*$K2*$U2*s(deg2rad($J2))*c(2*deg2rad($I2))-0.5*$U2*$U2*s(4*deg2rad($I2))-1.25*$K2*$K2*s(2*deg2rad($J2)))" | bc -l arcsin.bc`
V2=$EqOfTime
debug_pr "EqOfTime=$V2"

HaSunrise=`echo "rad2deg(acos(c(deg2rad(90.833))/(c(deg2rad($LAT))*c(deg2rad($T2)))-tan(deg2rad($LAT))*tan(deg2rad($T2))))" | bc -l arcsin.bc`
W2=$HaSunrise
debug_pr "HaSunrise=$W2"

SolarNoon=`echo "(720-4*$LONG-($V2))/1440" | bc -l arcsin.bc`
X2=$SolarNoon
debug_pr "SolarNoon=$X2"

SunriseTime=`echo "$X2-($W2*4/1440)" | bc -l `
Y2=$SunriseTime
debug_pr "SunriseTime=$Y2"

SunsetTime=`echo "$X2+$W2*4/1440" | bc -l arcsin.bc`
Z2=$SunsetTime
debug_pr "SunsetTime=$Z2"

SunlightDuration=`echo "8*$W2" | bc -l`
AA2=$SunlightDuration
debug_pr "SunlightDuration=$AA2"


TrueSolarTime=`echo "mod(($hours*60+$minutes)+$V2+4*$LONG,1440)" | bc -l arcsin.bc`
AB2=$TrueSolarTime
debug_pr "TrueSolarTime=$AB2"

res=`echo "is_bigger($AB2/4,0)" | bc -l arcsin.bc`
if (($res == 1 )); then
	HourAngle=`echo "$AB2/4-180" | bc -l`
else
	HourAngle=`echo "$AB2/4+180" | bc -l`
fi
AC2=$HourAngle
debug_pr "HourAngle=$AC2"

#SolarZenithAngle=`echo "rad2deg(arccos(s(deg2rad($LAT))*s(deg2rad($T2))+c(deg2rad($LAT))*c(deg2rad($T2))*c(deg2rad($AC2))))" | bc -l arcsin.bc`

SolarZenithAngle=`echo "rad2deg(acos(s(deg2rad($LAT))*s(deg2rad($T2))+c(deg2rad($LAT))*c(deg2rad($T2))*c(deg2rad($AC2))))" | bc -l arcsin.bc`

AD2=$SolarZenithAngle
debug_pr "SolarZenithAngle=$AD2"

SolarElevationAngle=`echo "90-($AD2)" | bc -l`
AE2=$SolarElevationAngle
debug_pr "SolarElevationAngle=$AE2"

res1=`echo "is_bigger($AE2,85)" | bc -l arcsin.bc `
res2=`echo "is_bigger($AE2,5)" | bc -l arcsin.bc `
res3=`echo "is_bigger($AE2,-0.575)" | bc -l arcsin.bc `
if (($res1 == 1)); then
	ApproxAtmosphericRefraction=0
elif (($res2 == 1)); then
	ApproxAtmosphericRefraction=`echo "(58.1/tan(deg2rad($AE2))-0.07/(tan(deg2rad($AE2))^3)+0.000086/(tan(deg2rad($AE2))^5))/3600" | bc -l arcsin.bc `
elif (($res3 == 1 )); then
	ApproxAtmosphericRefraction=`echo "(1735+$AE2*(-518.2+$AE2*(103.4+$AE2*(-12.79+$AE2*0.711))))/3600" | bc -l arcsin.bc`
else
	ApproxAtmosphericRefraction=`echo "-20.772/tan(deg2rad($AE2))/3600" | bc -l arcsin.bc`
fi
AF2=$ApproxAtmosphericRefraction
debug_pr "ApproxAtmosphericRefraction=$AF2"


SolarElevCorrForAtmRef=`echo "$AE2+$AF2" | bc -l`
AG2=$SolarElevCorrForAtmRef
debug_pr "SolarElevCorrForAtmRef=$AG2"

res=`echo "is_bigger($AC2,0)" | bc -l arcsin.bc`
if (($res == 1)); then
	SolarAzimuthAngle=`echo "mod(rad2deg(acos(((s(deg2rad($LAT))*c(deg2rad($AD2)))-s(deg2rad($T2)))/(c(deg2rad($LAT))*s(deg2rad($AD2)))))+180,360)" | bc -l arcsin.bc`
else
	SolarAzimuthAngle=`echo "mod(540-rad2deg(acos(((s(deg2rad($LAT))*c(deg2rad($AD2)))-s(deg2rad($T2)))/(c(deg2rad($LAT))*s(deg2rad($AD2))))),360)" | bc -l arcsin.bc`
fi
AH2=$SolarAzimuthAngle 
debug_pr "SolarAzimuthAngle=$AH2"

if [ "$RES" == "-E"  ]; then
#	echo $AG2
	printf "%0.f\n" $AG2
fi

if [  "$RES" == "-A" ]; then
#	echo $AH2
	printf "%0.f\n" $AH2
fi





