// This program was written by Jay Doscher (jay@polyideas.com)
// More information on this project can be found at www.polyideas.com
// This software is licensed under the NNNN license.
// NNNN Adafruit license stuff here

// Common Arduino library references
#include <math.h>

// magdec is your magnetic declination, which is the delta between true and magnetic north
// Enter this as a positive or negative number
// You can determine your local magnetic declination here: http://www.ngdc.noaa.gov/geomag-web/#declination
int magdec = -50;
// This is the margin of error for sensing the heading. Make it too big and the panel won't be accurate. 
// Make it too small and variations in the compass may make it miss when rotating.
//int headingMarginUpper = 2;
//int headingMarginLower = -2;

// Turning on DEBUGPrint will show you the Solarbot motor and logic messages
boolean DEBUGPrint  = true;


// Start solar variables, don't mess with these.
float JulianDay;
float JulianCentury;
float TrueSolarTime;
float EqOfTime;
float GeomMeanAnomSun;
float EccentEarthOrbit;
float GeomMeanLSunInt;
float GeomMeanLongSun;
float SunTrueLong;
float SunTrueAnom;
float SunRadVector;
float SunEqofCtr;
float SunAppLong;
float MeanObliqEcliptic;
float ObliqCorr;
float SunRtAscen;
float SunDeclin;
float VarY;
float HASunrise;
float HourAngle;
float SolarZenithAngle;
float CorrectedSolarElevation;
float ApproxAtmosRefraction;
float SolarElevation;
float SolarAzimuth;

//String motorDirection;
float fltHrs; 
float fltMins; 
float Latitude;
float Longitude;

void setup()  
{
     // GPS setup start
  Serial.begin(115200);                         //GPS needs this to be 115200
}

void loop()                     // The main loop
{
  int intDay = 3;
  int intMonth = 4;
  int intYear = 2014;
  int intHour = 16;
  int intMinute = 22;
  Longitude = -117.123;
  Latitude = 33.123;
  fltHrs = (intHour + (intMinute/60));
  fltMins = (fltHrs/24);


  //Calculate the Julian Day
//  JulianDay = (intDay + (fltHrs/24)) + (((153*(intMonth + (12 * ((14-intMonth)/12)) -3))+2)/5) + (365 * ((intYear + 4800) - ((14-intMonth)/12))) + (((intYear + 4800) - ((14-intMonth)/12))/4) - (((intYear + 4800) - ((14-intMonth)/12))/100) + (((intYear + 4800) - ((14-intMonth)/12))/400) -32045;
JulianDay = 2456751.18;
  Serial.print("JulianDay=");
  Serial.println(JulianDay);


  JulianCentury = (JulianDay-2451545)/36525;
//JulianCentury = 0.14253749;

  Serial.print("JulianCentury=");
  Serial.println(JulianCentury); 

  GeomMeanLongSun =fmod(280.46646 + JulianCentury * (36000.76983 + JulianCentury * 0.0003032), 360);
  Serial.print("GeomMeanLongSun=");
  Serial.println(GeomMeanLongSun); 

  GeomMeanAnomSun = (357.52911 + JulianCentury * (35999.05029 - 0.0001537 * JulianCentury));
  Serial.print("GeomMeanAnomSun=");
  Serial.println(GeomMeanAnomSun); 

  EccentEarthOrbit = (0.016708634 - JulianCentury * (0.000042037 + 0.0000001267 * JulianCentury));
  Serial.print("EccentEarthOrbit=");
  Serial.println(EccentEarthOrbit); 

  SunEqofCtr = sin(radians(GeomMeanAnomSun))*(1.914602-JulianCentury*(0.004817+0.000014*JulianCentury))+sin(radians(2*GeomMeanAnomSun))*(0.019993-0.000101*JulianCentury)+sin(radians(3*GeomMeanAnomSun))*0.000289;
  Serial.print("SunEqofCtr=");
  Serial.println(SunEqofCtr); 

  SunTrueLong = GeomMeanLongSun + SunEqofCtr;
  Serial.print("SunTrueLong=");
  Serial.println(SunTrueLong); 

  SunTrueAnom = GeomMeanAnomSun + SunEqofCtr;
  Serial.print("SunTrueAnom=");
  Serial.println(SunTrueAnom); 

  SunRadVector = (1.000001018*(1-EccentEarthOrbit*EccentEarthOrbit))/(1+EccentEarthOrbit*cos(radians(SunTrueAnom)));
  Serial.print("SunRadVector=");
  Serial.println(SunRadVector); 

  SunAppLong = SunTrueLong-0.00569-0.00478*sin(radians(125.04-1934.136*JulianCentury));
  Serial.print("SunAppLong=");
  Serial.println(SunAppLong); 

  MeanObliqEcliptic = 23+(26+((21.448-JulianCentury*(46.815+JulianCentury*(0.00059-JulianCentury*0.001813))))/60)/60;
  Serial.print("MeanObliqEcliptic=");
  Serial.println(MeanObliqEcliptic); 

  ObliqCorr = MeanObliqEcliptic+0.00256*cos(radians(125.04-1934.136*JulianCentury));
  Serial.print("ObliqCorr=");
  Serial.println(ObliqCorr); 


SunRtAscen=`echo "(atan2(c($R2*$PI/180),c($P2*$PI/180))*s($P2*$PI/180))*180/$PI" | bc -l atan2.bc`
=DEGREES(ATAN2(COS(RADIANS(P2)),COS(RADIANS(R2))*SIN(RADIANS(P2))))

  SunRtAscen = degrees(atan2(cos(radians(SunAppLong)),cos(radians(ObliqCorr))*sin(radians(SunAppLong))));
  Serial.print("SunRtAscen=");
  Serial.println(SunRtAscen); 

  SunDeclin = degrees(asin(sin(radians(ObliqCorr))*sin(radians(SunAppLong))));
  Serial.print("SunDeclin=");
  Serial.println(SunDeclin); 

  VarY =tan(radians(ObliqCorr/2))*tan(radians(ObliqCorr/2));
  Serial.print("VarY=");
  Serial.println(VarY); 

  EqOfTime= 4*degrees(VarY*sin(2*radians(GeomMeanLongSun))-2*EccentEarthOrbit*sin(radians(GeomMeanAnomSun))+4*EccentEarthOrbit*VarY*sin(radians(GeomMeanAnomSun))*cos(2*radians(GeomMeanLongSun))-0.5*VarY*VarY*sin(4*radians(GeomMeanLongSun))-1.25*EccentEarthOrbit*EccentEarthOrbit*sin(2*radians(GeomMeanAnomSun)));
  Serial.print("EqOfTime=");
  Serial.println(EqOfTime); 

  HASunrise = degrees(acos(cos(radians(90.833))/(cos(radians(Latitude))*cos(radians(SunDeclin)))-tan(radians(Latitude))*tan(radians(SunDeclin))));
  Serial.print("HASunrise=");
  Serial.println(HASunrise); 

  TrueSolarTime = fmod((fltMins*1440)+EqOfTime+(4*Longitude),1440);
  Serial.print("TrueSolarTime=");
  Serial.println(TrueSolarTime); 

  if (TrueSolarTime/4<0){ 
    HourAngle = (TrueSolarTime/4+180);
  }
  else
  {
    HourAngle = (TrueSolarTime/4-180);
  }
  SolarZenithAngle = degrees(acos(sin(radians(Latitude))*sin(radians(SunDeclin))+cos(radians(Latitude))*cos(radians(SunDeclin))*cos(radians(HourAngle))));
  Serial.print("SolarZenithAngle=");
  Serial.println(SolarZenithAngle); 

  SolarElevation = 90-SolarZenithAngle;
  Serial.print("SolarElevation=");
  Serial.println(SolarElevation); 

  if (SolarElevation>85) {
    ApproxAtmosRefraction =0;
  }
  else if (SolarElevation<85) {
    ApproxAtmosRefraction = (58.1/tan(radians(SolarElevation))-0.07/pow(tan(radians(SolarElevation)),3)+0.000086/pow(tan(radians(SolarElevation)),5))/3600;
  }
  else if (SolarElevation>-0.575) {
    ApproxAtmosRefraction = 1735+SolarElevation*(-518.2+SolarElevation*(103.4+SolarElevation*(-12.79+SolarElevation*0.711))),-20.772/tan(radians(SolarElevation))/3600;
  }


  CorrectedSolarElevation = round(SolarElevation+ApproxAtmosRefraction);
  Serial.print("CorrectedSolarElevation=");
  Serial.println(CorrectedSolarElevation); 

  if (HourAngle>0) {
    SolarAzimuth = (fmod(degrees(acos(((sin(radians(Latitude))*cos(radians(SolarZenithAngle)))-sin(radians(SunDeclin)))/(cos(radians(Latitude))*sin(radians(SolarZenithAngle)))))+180,360));
  }	
  else {
    SolarAzimuth = (fmod(540-degrees(acos(((sin(radians(Latitude))*cos(radians(SolarZenithAngle)))-sin(radians(SunDeclin)))/(cos(radians(Latitude))*sin(radians(SolarZenithAngle))))),360));
  }

  Serial.print("SolarAzimuth=");
  Serial.println(SolarAzimuth); 



  //Adjust the magnetic declination
  if (CorrectedSolarElevation > 0)
  {
    if (magdec < 0)
    {
      //      Serial.print(SolarAzimuth);
      //      Serial.print("->");
      if (SolarAzimuth + magdec >= 0)
      {
        SolarAzimuth = SolarAzimuth + magdec;
        //              Serial.println(SolarAzimuth);
      }
      else
      {
        SolarAzimuth = (SolarAzimuth + magdec + 360);
      }
    } 
    else if (magdec > 0)
    {
      //      Serial.print(SolarAzimuth);
      //      Serial.print("->");
      if (SolarAzimuth + magdec <= 360)
      {
        SolarAzimuth = SolarAzimuth + magdec;
        Serial.println(SolarAzimuth);
      }
      else
      {
        SolarAzimuth = (SolarAzimuth + magdec - 360);
        //                      Serial.println(SolarAzimuth);
      }  
    }
  }

Serial.println("**************************");
  delay(100000); //Delay 2 minutes
}














