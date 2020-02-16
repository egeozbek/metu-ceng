#include "Transporter.hpp"
#include "writeOutput.h"

Transporter::Transporter(TransporterInfo * transporterInfo,unsigned sleepTime):transporterInfo(transporterInfo),transporterSleepTime(sleepTime)
{
  lastCheckedMiner=-1;
  targetSmelter = -1 ;
  targetFoundry = -1;
}
